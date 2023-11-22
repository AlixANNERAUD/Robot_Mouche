#ifndef I2C_HPP_INCLUDED
#define I2C_HPP_INCLUDED

#include "pin.hpp"
#include "lidar.hpp"
#include "motor.hpp"
#include "QTRSensors.hpp"
#include "settings.hpp"
#include "pin.hpp"
#include "pid.hpp"

class DriverClass
{
public:
    DriverClass() = delete;
    DriverClass(MotorClass &left, MotorClass &right, QTRSensors &qtr, SettingsClass &settings);
    
    void run();
    void stop();

    void setSpeed(float speed);
    void setDirection(float direction);

private:
    bool running;
    MotorClass &left;
    MotorClass &right;
    QTRSensors &qtr;
    SettingsClass &settings;

    PidControlClass pid;

    float speed;
    float steering;
    float linePosition;

    float computeLinePosition();
};

#endif