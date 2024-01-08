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
        // Return 42 69 if file not found to prevent client bug
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
        char max = 0;
        for (int j = range[0]; j < range[1]; j++)
        {
            if (values[j] > max)
            {
                max = values[j];
            }
        }
        linePosition[i] = max > 100;
    }

    return linePosition;
}


void DriverClass::update()
{
    if (this->settings.mode != RobotMode::LineFollower)
        return;

    if (this->lidar.getDistance() < 5 && this->lidar.getDistance() > 0)
    {
        this->setMotorsSpeed(0, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return;
    }

    auto values = readLinePositionFile();
    while (true)
    {
        // Check if file is corrupted
        if (values[0] == 42 && values[1] == 69)
        {
            values = readLinePositionFile();
        } else {
            break;
        }
    }

    static double failAngle = M_PI_2;

    // Compute mean
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
            {
                if (i < 50) {
                    mean -= i;
                } else {
                    mean -= i;
                }
            }
            else
            {
                if (i > 590) {
                    mean += i;
                } else {
                    mean += i;
                }

            }
        }
    }
    mean /= 640 * (640 + 1) / 2;

    double angle;

    if (found)
    {
        // Compute angle
        angle = M_PI - (mean * M_PI);
        failAngle = (failAngle * 0.9) + (angle * 0.1);
        angle = failAngle;
    }
    else
    {
        // Compute angle if line not found
        angle = failAngle;
        if (failAngle < M_PI_2)
            angle = 0;
        else
            angle = M_PI;
        LOG_WARNING("Driver", "Line not found");
    }

    // Check if angle is valid
    if (angle <= 0)
    {
        //LOG_WARNING("Driver", "Angle < 0");
        angle = 0 + (M_PI_4 / 2);
    }
    else if (angle >= M_PI)
    {
        //LOG_WARNING("Driver", "Angle > PI");
        angle = M_PI - (M_PI_4 / 2);
    }

    //LOG_INFORMATION("Driver", "Mean: %f -> Angle: %f", mean, angle);

    // std::array<bool, 5> linePosition = this->computeLinePosition(values);

    float magnitude = 0.15;

    //  float angle = -j * M_PI_4 + M_PI;

    // LOG_INFORMATION("Driver", "j : %i - Driving angle: %f", j, angle);
    this->setSpeedFromPolarCoordinates(magnitude, angle);

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
    // theta = fmod(theta + 2 * M_PI, 2 * M_PI);
    float turnDamping = 1;

    // Compute speed for each motor from polar coordinates (r, theta)
    float leftSpeed = r * (std::sin(theta) + std::cos(theta) / turnDamping);
    float rightSpeed = r * (std::sin(theta) - std::cos(theta) / turnDamping);

    this->setMotorsSpeed(leftSpeed, rightSpeed);
}

void DriverClass::setSpeedFromCartesianPosition(float x, float y)
{
    float r = sqrt(x * x + y * y);
    float theta = atan2(y, x);

    float maxR = 1;

    if (std::abs(x) > std::abs(y))
    {
        maxR = std::abs(r / x);
    }
    else
    {
        maxR = std::abs(r / y);
    }

    r = r / maxR;

    this->setSpeedFromPolarCoordinates(r, theta);
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
