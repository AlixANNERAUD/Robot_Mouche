#include "driver.hpp"
#include "log.hpp"
#include <algorithm>
#include <chrono>
#include <array>
#include <thread>
#include <iostream>
#include <fstream>
#include <signal.h>

using namespace std;

DriverClass::DriverClass(LiDARClass& lidar, MotorClass &left, MotorClass &right)
    : running(false), left(left), right(right), settings(settings), pid(settings.KP, settings.KI, settings.KD, 0.0), mode(RobotMode::Manual), lidar(lidar)
{
    this->speed = 0.0f;
    this->steering = 0.0f;
}

void DriverClass::start()
{
    if (this->running)
        return;

    std::thread(&DriverClass::run, this).detach();
}

void DriverClass::run()
{
    this->running = true;
    while (this->running)
    {
        this->update();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    this->left.setSpeed(0);
    this->right.setSpeed(0);
}

double DriverClass::computeLinePosition()
{
    // Open file line_position.txt
    fstream file;
    file.open("line_position.bin", ios::in);
    if (!file.is_open())
    {
        LOG_ERROR("Driver", "Failed to open line_position.bin");
        return 0.0;
    }

    // Read 640 comma separated values
    char values[640];
    file.read(values, sizeof(values));

    // Find the peak
    int peakIndex = 0;
    int peakValue = 0;
    for (int i = 0; i < 640; i++)
    {
        if (values[i] > peakValue)
        {
            peakValue = values[i];
            peakIndex = i;
        }
    }

    // If there is no peak go straight
    if (peakValue < 100)
    {
        LOG_DEBUG("Driver", "No peak found");
        return 0.0;
    }

    // Ensure there is no other peak
    for (int i = 0; i < 640; i++)
    {
        if (i + 50 > peakIndex && i < peakIndex + 50)
            continue;
        if (values[i] > peakValue * 2 / 3)
        {
            LOG_ERROR("Driver", "Found multiple peaks");
            return 0.0;
        }
    }

    double position = ((double)peakIndex - 320.0) / 640.0;
    LOG_DEBUG("Driver", "Line position : %f", position);

    return position;
}

void DriverClass::update()
{
    this->linePosition = this->computeLinePosition();
    LOG_DEBUG("Driver", "Line position : %f", this->linePosition);

    if (this->mode == RobotMode::LineFollower)
    {
        float steering = (float)this->pid.getSteering(this->linePosition, clock());
        LOG_DEBUG("Driver", "Steering before processing : %f", steering);
        this->steering = std::clamp(steering / 1024.0f, -1.0f, 1.0f); // NORMALIZE it from -1.0 to 1.0
        LOG_DEBUG("Driver", "Steering after processing : %f", this->steering);
        float speedLeft = std::min(this->speed - this->steering, 1.0f) * 1024.0f;
        float speedRight = std::min(this->speed - this->steering, 1.0f) * 1024.0f;

        if (this->mode == RobotMode::LineFollower && this->lidar.getDistance() < 100)
        {
            speedLeft = 0;
            speedRight = 0;
        }
        
        this->left.setSpeed((unsigned int)speedLeft);
        this->right.setSpeed((unsigned int)speedRight);
    }
}

void DriverClass::stop()
{
    this->running = false;
}

void DriverClass::setSpeed(float speed)
{
    if (speed >= 0)
    {
        this->speed = clamp(speed, 0.0f, 1.0f);
        this->right.set(MotorDirection::Forward, (unsigned int)(speed * 1024.0f));
        this->left.set(MotorDirection::Forward, (unsigned int)(speed * 1024.0f));
    }
    else
    {
        this->speed = clamp(-speed, 0.0f, 1.0f);
        this->right.set(MotorDirection::Backward, (unsigned int)(-speed * 1024.0f));
        this->left.set(MotorDirection::Backward, (unsigned int)(-speed * 1024.0f));
    }
}

void DriverClass::setDirection(float direction)
{
    this->steering = clamp(direction, -1.0f, 1.0f);
}

void DriverClass::updateGamepad(float direction, float speed)
{
    if (this->settings.mode == RobotMode::LineFollower)
        return;



    float velocity = speed;
    float rotation = direction;

    float left_speed = velocity - rotation;
    float right_speed = velocity + rotation;

    printf("Left speed : %f\n", left_speed);
    printf("Right speed : %f\n", right_speed);

    if (left_speed >= 0.0f) {
        this->left.set(MotorDirection::Forward, (unsigned int)(left_speed * 1024.0f));
    }
    else {
        this->left.set(MotorDirection::Backward, (unsigned int)(-left_speed * 1024.0f));
    }

    if (right_speed >= 0.0f) {
        this->right.set(MotorDirection::Forward, (unsigned int)(right_speed * 1024.0f));
    }
    else {
        this->right.set(MotorDirection::Backward, (unsigned int)(-right_speed * 1024.0f));
    }

    //this->setSpeed(speed);
    //this->setDirection(direction);
}

void DriverClass::updateSettings(SettingsClass settings)
{
    // Check is changing mode
    #ifdef RASPBERRY_PI
    if (settings.mode != this->settings.mode)
    {        
        if (settings.mode == RobotMode::LineFollower)
        {
            // Start camera program
            this->camera_program_pid = fork();
            if (this->camera_program_pid == 0)
            {
                execl("/usr/bin/python3", "/home/pi/Documents/Robot_Mouche/code/src/cam2.py", NULL);
                exit(0);
            }
        }
        else
        {
            // Stop camera program
            kill(this->camera_program_pid, SIGKILL);
        }
    }
    #endif 
    this->settings = settings;
    this->pid.updateConstants(settings.KP, settings.KI, settings.KD);
       
}
