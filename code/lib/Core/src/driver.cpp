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

std::array<bool, 5> DriverClass::computeLinePosition(std::array<char, 640> values)
{
    int ranges[5][2] = {{0, 80}, {80, 200}, {200, 440}, {440, 560}, {560, 640}};

    std::array<bool, 5> linePosition;
    for (int i = 0; i < 5; i++)
    {
        int range[2] = {ranges[i][0], ranges[i][1]};
        double sum = 0.0;
        for (int j = range[0]; j < range[1]; j++)
        {
            sum += (double)values[j];
        }
        double average = sum / (double)(range[1] - range[0]);
        if (average > 100)
        {
            linePosition[i] = true;
        }
        else
        {
            linePosition[i] = false;
        }
    }

    return linePosition;
}

void DriverClass::update()
{
    auto values = readLinePositionFile();
    if (values[0] == 42 && values[1] == 69) {
        return;
    }
    std::array<bool, 5> linePosition = this->computeLinePosition(values);

    LOG_DEBUG("Driver", "Line position : %d %d %d %d %d", linePosition[0], linePosition[1], linePosition[2], linePosition[3], linePosition[4]);

    int eq = 0;
    if (linePosition[4])
        eq += 1;
    if (linePosition[3])
        eq += 0b10;
    if (linePosition[2])
        eq += 0b100;
    if (linePosition[1])
        eq += 0b1000;
    if (linePosition[0])
        eq += 0b10000;

    switch (eq)
    {
    case 0b10000:
    case 0b11000:
    case 0b11100:
    case 0b10100:
    case 0b01000:
        this->setMotorsSpeed(this->settings.KS/2, this->settings.KS);
        break;
    case 0b01100:
        this->setMotorsSpeed(this->settings.KS/4, this->settings.KS/2);
        break;
    case 0b00001:
    case 0b00011:
    case 0b00111:
    case 0b00101:
    case 0b00010:
        this->setMotorsSpeed(this->settings.KS, this->settings.KS/2);
        break;
    case 0b00110:
        this->setMotorsSpeed(this->settings.KS/2, this->settings.KS/4);
        break;
    case 0b00100:
    case 0b01110:
    case 0b11111:
        this->setMotorsSpeed(this->settings.KS, this->settings.KS);
        break;
    
    
    default:
        this->setMotorsSpeed(this->settings.KS, this->settings.KS);
        break;
    }

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
