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
    DriverClass(MotorClass &left, MotorClass &right, QTRClass &qtr1, QTRClass &qtr2, SettingsClass &settings);
    
    void start();
    void stop();

    void setSpeed(float speed);
    void setDirection(float direction);

    void updatePidConstants(SettingsClass settings);

    double computeLinePosition();

private:
    std::atomic_bool running;
    MotorClass &left;
    MotorClass &right;
    QTRClass &qtr1;
    QTRClass &qtr2;
    SettingsClass &settings;

    PidControlClass pid;

    float speed;
    float steering;
    float linePosition;

    void run();
    void update();
};

#endif
