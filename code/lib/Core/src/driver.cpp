#include "driver.hpp"
#include "log.hpp"
#include "sound.hpp"

#include <algorithm>
#include <chrono>
#include <array>
#include <thread>
#include <iostream>
#include <fstream>
#include <signal.h>
#include <cmath>

// Je suis libertin!

DriverClass::DriverClass(LiDARClass &lidar, MotorClass &left, MotorClass &right, LCDClass &lcd)
    : running(false), left(left), right(right), settings(settings), pid(settings.KP, settings.KI, settings.KD, M_PI_2), lidar(lidar), lcd(lcd)
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
    this->lcd.print(0, "- Robot Mouche -");
    this->running = true;
    bool soundPlayed = false;
    while (this->running)
    {
        if (this->lidar.getDistance() > 0)
        {
            // - Remember last distance to avoid to much lcd update
            if (this->lidar.getDistance() < 5)
            {
                this->lcd.print(1, "   Obstacle!    ");
                if (!soundPlayed)
                {
                    SoundClass::play("pig3.mp3");
                    soundPlayed = true;
                }
            }
            else
            {
                this->lcd.printFormatted(1, "D: %d cm  ", this->lidar.getDistance());
                soundPlayed = false;
            }
        }
        this->followLine();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    this->setMotorsSpeed(0, 0);
}

float DriverClass::findLinePosition()
{
    static double failMean = (640 * (640 + 1) / 2) / 2; // Moving average of the last means
    // - Read line position file
    std::array<uint8_t, 640> values;
    while (!readLinePositionFile(values))
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // - Wait for file to be ready

    // - Compute mean
    double mean = (640 * (640 + 1) / 2) / 2;
    bool found = false;
    for (int i = 0; i < (640); i++)
    {
        // Check if value is above threshold
        if (values[i] > 130)
        {
            found = true;
            // Check if value is on the left or right side
            if (i < 640 / 2)
                mean -= i;
            else
                mean += i;
        }
    }
    mean /= 640 * (640 + 1) / 2; // Normalize mean

    // - If line found, update failMean and return mean
    if (found)
    {
        failMean = (failMean * 0.9) + (mean * 0.1);
        return mean;
    }
    // - If line not found, return extreme value from failMean

    if (failMean < 640 / 2)
        return 0;
    else
        return 640 * (640 + 1) / 2; // Maximum mean value
}

bool DriverClass::readLinePositionFile(std::array<uint8_t, 640> &values)
{
    // Open file line_position.txt
    std::fstream file;
    file.open("line_position.bin", std::ios::in);
    if (!file.is_open()) // This happen frequently
    {
        LOG_ERROR("Driver", "Failed to open line_position.bin");
        return false;
    }
    // Read 640 values from file
    file.read((char *)values.data(), values.size());
    file.close();
    return true;
}

void DriverClass::followLine()
{
    if (this->settings.mode != RobotMode::LineFollower)
        return;

    if (this->lidar.getDistance() < 5 && this->lidar.getDistance() > 0)
    {
        this->setMotorsSpeed(0, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return;
    }

    double angle = M_1_PI - (DriverClass::findLinePosition() * M_PI); // Convert line position to angle

    this->setSpeedFromPolarCoordinates(0.15, angle);

    return;
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

void DriverClass::setSpeedFromPolarCoordinates(float r, float theta)
{
    float turnDamping = 1;

    // Compute speed for each motor from polar coordinates (r, theta)
    float leftSpeed = r * (std::sin(theta) + std::cos(theta) / turnDamping);
    float rightSpeed = r * (std::sin(theta) - std::cos(theta) / turnDamping);

    this->setMotorsSpeed(leftSpeed, rightSpeed);
}

void DriverClass::setSpeedFromCartesianCoordinates(float x, float y)
{
    float r = sqrt(x * x + y * y);
    float theta = atan2(y, x);

    float maxR = 1;
    if (std::abs(x) > std::abs(y))
        maxR = std::abs(r / x);
    else
        maxR = std::abs(r / y);
    r = r / maxR;

    this->setSpeedFromPolarCoordinates(r, theta);
}

void DriverClass::updateGamepad(float direction, float speed)
{
    if (this->settings.mode == RobotMode::LineFollower)
        return;

    this->setSpeedFromCartesianCoordinates(direction, -speed);
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
