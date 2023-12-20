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

// Je suis libertin!

DriverClass::DriverClass(LiDARClass &lidar, MotorClass &left, MotorClass &right)
    : running(false), left(left), right(right), settings(settings), pid(settings.KP, settings.KI, settings.KD, 0.0), lidar(lidar)
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

std::array<char, 640> DriverClass::readLinePositionFile()
{
    // Open file line_position.txt
    std::fstream file;
    file.open("line_position.bin", std::ios::in);

    std::array<char, 640> values;
    if (!file.is_open())
    {
        LOG_ERROR("Driver", "Failed to open line_position.bin");
        values[0] = 42;
        values[1] = 69;
        return values;
    }

    // Read 640 values
    file.read(values.data(), values.size());
    return values;
}

double DriverClass::computeLinePosition(std::array<char, 640> values)
{
    int line_start = -1;
    int line_end = 0;
    int limit = 100;
    while (limit--) {
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
        line_end = 639;
        for (int i = line_start; i < 640; i++)
        {
            if (values[i] < 100 && (i >= 638 || values[i + 1] < 100) && (i >= 637 || values[i + 2] < 100))
            {
                line_end = i;
                break;
            }
        }

        // If line width is more than 50, accept the line or else search another
        if (line_end - line_start > 50)
        {
            break;
        } else
        {
     //       LOG_ERROR("Driver", "Line width is too small (%d)", line_end - line_start);
        }
    }

    int line_position = (line_start + line_end) / 2;
    double position = ((double)line_position - 320.0) / 320.0;
    //LOG_DEBUG("Driver", "Line position : %d", line_position);

    if (!limit) {
        LOG_ERROR("Driver", "Failed to find line");
        return 0.0;
    }
    return position;
}

void DriverClass::update()
{
    auto values = readLinePositionFile();
    if (values[0] == 42 && values[1] == 69) {
        return;
    }
    this->linePosition = this->computeLinePosition(values);
    //LOG_DEBUG("Driver", "Line position : %f", this->linePosition);

    if (this->settings.mode == RobotMode::LineFollower)
    {
        this->steering = (float)this->linePosition;
       // float steering = (float)this->pid.getSteering(this->linePosition, clock());
        //LOG_DEBUG("Driver", "Steering before processing : %f", steering);
        this->steering = std::clamp(this->steering, -1.0f, 1.0f)*0.1; // NORMALIZE it from -1.0 to 1.0
        

        //LOG_DEBUG("Driver", "Steering after processing : %f", this->steering);
        float speedLeft = std::min(this->speed - this->steering, 1.0f) * 1024.0f;
        float speedRight = std::min(this->speed - this->steering, 1.0f) * 1024.0f;

        if (this->settings.mode == RobotMode::LineFollower && this->lidar.getDistance() < 100)
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
    this->left.set(left);
    this->right.set(right);
}

void DriverClass::setSpeedFromCartesianPosition(float x, float y)
{
    float r = sqrt(x * x + y * y);
    float theta = atan2(y, x);

    // theta = fmod(theta + 2 * M_PI, 2 * M_PI);

    float maxR = 1;

    if (std::abs(x) > std::abs(y))
    {
        maxR = std::abs(r / x);
    }
    else
    {
        maxR = std::abs(r / y);
    }

    float magnitude = r / maxR;

    float turnDamping = 1;

    float leftSpeed = magnitude * (std::sin(theta) + std::cos(theta) / turnDamping);
    float rightSpeed = magnitude * (std::sin(theta) - std::cos(theta) / turnDamping);

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
    LOG_DEBUG("Driver", "Updating settings");
    // Check is changing mode
    if (settings.mode != this->settings.mode)
    {
        LOG_DEBUG("Driver", "Changing mode");
        if (settings.mode == RobotMode::LineFollower)
        {
#ifdef RASPBERRY_PI
            // Start camera program
            this->camera_program_pid = fork();
            LOG_INFORMATION("Driver", "Starting camera program");
            if (this->camera_program_pid == 0)
            {
                if (execl("/usr/bin/python3", "/usr/bin/python3", "/home/pi/Documents/Robot_Mouche/code/src/cam2.py") != 0)
                    LOG_ERROR("Driver", "Failed to start camera program");

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
