#ifndef DRIVER_HPP_INCLUDED
#define DRIVER_HPP_INCLUDED

#include "lidar.hpp"
#include "motor.hpp"
#include "settings.hpp"
#include "pin.hpp"
#include "pid.hpp"

#include <array>
#include <unistd.h>
#include <atomic>

class DriverClass
{
public:
    DriverClass() = delete;
    DriverClass(LiDARClass& lidar, MotorClass &left, MotorClass &right);
    
    void start();
    void stop();

    void setMotorsSpeed(float left, float right);
    void setSpeedFromCartesianPosition(float x, float y);

    // - Callbacks
    void updateSettings(SettingsClass settings);
    void updateGamepad(float direction, float speed);

    static std::array<bool, 5> computeLinePosition(std::array<char, 640> values);
    static std::array<char, 640> readLinePositionFile();

private:    
    std::atomic_bool running;
    MotorClass &left;
    MotorClass &right;
    LiDARClass &lidar;
    SettingsClass settings;

    PidControlClass pid;

    float speed;
    float steering;
    float linePosition;
    float lastPositionKnown;
    
    int lastDecision = 0;

    clock_t cycleStart = 0;
    int backwardCount = 0;

    pid_t camera_program_pid;

    void run();
    void update();

    int takeDecision(int mask);
};

#endif
