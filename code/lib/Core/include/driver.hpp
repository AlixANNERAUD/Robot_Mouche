#ifndef DRIVER_HPP_INCLUDED
#define DRIVER_HPP_INCLUDED

#include "pin.hpp"
#include "lidar.hpp"
#include "motor.hpp"
#include "qtr.hpp"
#include "settings.hpp"
#include "pin.hpp"
#include "pid.hpp"

class DriverClass
{
public:
    DriverClass() = delete;
    DriverClass(MotorClass &left, MotorClass &right, QTRClass &qtr1, QTRClass &qtr2, SettingsClass &settings);
    
    void run();
    void stop();

    void setSpeed(float speed);
    void setDirection(float direction);

    double computeLinePosition();

private:
    bool running;
    MotorClass &left;
    MotorClass &right;
    QTRClass &qtr1;
    QTRClass &qtr2;
    SettingsClass &settings;

    PidControlClass pid;

    float speed;
    float steering;
    float linePosition;

    void update();
};

#endif