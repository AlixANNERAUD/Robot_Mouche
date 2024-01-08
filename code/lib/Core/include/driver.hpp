#pragma once

#include "lidar.hpp"
#include "motor.hpp"
#include "settings.hpp"
#include "pin.hpp"
#include "pid.hpp"
#include "lcd.hpp"

#include <array>
#include <unistd.h>
#include <atomic>

class DriverClass
{
public:
    DriverClass() = delete;
    DriverClass(LiDARClass &lidar, MotorClass &left, MotorClass &right, LCDClass &lcd);

    void start();
    void stop();

    // - Callbacks
    void updateSettings(SettingsClass settings);
    void updateGamepad(float direction, float speed);

    // static std::array<bool, 5> computeLinePosition(std::array<char, 640> values);

private:
    std::atomic_bool running;
    MotorClass &left;
    MotorClass &right;
    SettingsClass settings;
    LiDARClass &lidar;
    LCDClass &lcd;

    float speed;
    float steering;
    float linePosition;
    float lastPositionKnown;

    int lastDecision = 0;

    clock_t cycleStart = 0;
    clock_t lastBackward = 0;
    int backwardCount = 0;

    pid_t camera_program_pid;

    void run();
    void followLine();

    void setMotorsSpeed(float left, float right);
    void setSpeedFromCartesianCoordinates(float x, float y);
    void setSpeedFromPolarCoordinates(float r, float theta);

    static float findLinePosition();
    static bool readLinePositionFile(std::array<uint8_t, 640>& values);

    friend class ServerClass;
};
