#ifndef DRIVER_HPP_INCLUDED
#define DRIVER_HPP_INCLUDED

#include "lidar.hpp"
#include "motor.hpp"
#include "qtr.hpp"
#include "settings.hpp"
#include "pin.hpp"
#include "pid.hpp"

#include <atomic>

class DriverClass
{
public:
    DriverClass() = delete;
    DriverClass(MotorClass &left, MotorClass &right, QTRClass &qtr1, QTRClass &qtr2);
    
    void start();
    void stop();

    void setSpeed(float speed);
    void setDirection(float direction);

    // - Callbocks
    void updatePidConstants(SettingsClass settings);
    void updateGamepad(float direction, float speed);

    double computeLinePosition();

private:
    RobotMode mode;

    std::atomic_bool running;
    MotorClass &left;
    MotorClass &right;
    QTRClass &qtr1;
    QTRClass &qtr2;
    SettingsClass settings;

    PidControlClass pid;

    float speed;
    float steering;
    float linePosition;

    void run();
    void update();
};

#endif
