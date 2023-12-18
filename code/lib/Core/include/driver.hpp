#ifndef DRIVER_HPP_INCLUDED
#define DRIVER_HPP_INCLUDED

#include "lidar.hpp"
#include "motor.hpp"
#include "settings.hpp"
#include "pin.hpp"
#include "pid.hpp"

#include <atomic>

class DriverClass
{
public:
    DriverClass() = delete;
    DriverClass(LiDARClass& lidar, MotorClass &left, MotorClass &right);
    
    void start();
    void stop();

    void setSpeed(float speed);
    void setDirection(float direction);

    // - Callbacks
    void updatePidConstants(SettingsClass settings);
    void updateGamepad(float direction, float speed);

    double computeLinePosition();

private:
    RobotMode mode;

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

    void run();
    void update();
};

#endif
