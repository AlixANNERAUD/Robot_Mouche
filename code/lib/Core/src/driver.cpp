#include "driver.hpp"
#include "log.hpp"
#include <algorithm>
#include <chrono>
#include <array>
#include <thread>
#include <iostream>
#include <fstream>
#include <signal.h>
#include <cmath>

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
    std::fstream file;
    file.open("line_position.bin", std::ios::in);
    if (!file.is_open())
    {
        LOG_ERROR("Driver", "Failed to open line_position.bin");
        return 0.0;
    }

    // Read 640 comma separated values
    char values[640];
    file.read(values, sizeof(values));

    int line_start = -1;
    int line_end = 0;
    while (1) {
        // Find line start (x where value > 100)
        for (int i = line_end; i < 640; i++)
        {
            if (values[i] >= 100 && (i >= 638 || values[i + 1] >= 100) && (i >= 637 || values[i + 2] >= 100))
            {
                line_start = i;
                break;
            }
        }

        // Check line was found
        if (line_start == -1)
        {
            LOG_ERROR("Driver", "Failed to find line");
            return 0.0;
        }

        // Find line end
        for (int i = line_start; i < 640; i++)
        {
            if (values[i] < 100 && (i >= 638 || values[i + 1] < 100) && (i >= 637 || values[i + 2] < 100))
            {
                line_end = i;
                break;
            }
        }

        // If line width is more than 50 search another
        if (line_end - line_start < 50)
        {
            LOG_ERROR("Driver", "Line width is too small (%d)", line_end - line_start);
            continue;
        }

        // Ensure there is no other line
        for (int i = line_end; i < 640; i++)
        {
            if (values[i] >= 100 && (i >= 638 || values[i + 1] >= 100) && (i >= 637 || values[i + 2] >= 100))
            {
                LOG_ERROR("Driver", "Found another line");
                break;
            }
        }
    }

    int line_position = (line_start + line_end) / 2;
    double position = ((double)line_position - 320.0) / 320.0;
    LOG_DEBUG("Driver", "Line position : %d", line_position);

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

        this->setMotorsSpeed(speedLeft, speedRight);
    }
}

void DriverClass::stop()
{
    this->running = false;
}

void DriverClass::setMotorsSpeed(float left, float right)
{
    this->left.setSpeed(left);
    this->right.setSpeed(right);
}

void DriverClass::setSpeedFromCartesianPosition(float x, float y)
{
    float r = std::sqrt(x * x + y * y);
    float theta = std::atan2(y, x);

    float leftSpeed = r * std::cos(theta);
    float rightSpeed = r * std::sin(theta);


    this->setMotorsSpeed(leftSpeed, rightSpeed);
}

void DriverClass::updateGamepad(float direction, float speed)
{
    if (this->settings.mode == RobotMode::LineFollower)
        return;

    this->setSpeedFromCartesianPosition(direction, -speed);
}

void DriverClass::updateSettings(SettingsClass settings)
{
    // Check is changing mode
    if (settings.mode != this->settings.mode)
    {        
        if (settings.mode == RobotMode::LineFollower)
        {
            #ifdef RASPBERRY_PI
            // Start camera program
            this->camera_program_pid = fork();
            if (this->camera_program_pid == 0)
            {
                execl("/usr/bin/python3", "/home/pi/Documents/Robot_Mouche/code/src/cam2.py", NULL);
                exit(0);
            }
            #endif 
        }
        else
        {
            #ifdef RASPBERRY_PI
            // Stop camera program
            kill(this->camera_program_pid, SIGKILL);
            #endif 
        }
    }
    this->settings = settings;
    this->pid.updateConstants(settings.KP, settings.KI, settings.KD);
       
}
