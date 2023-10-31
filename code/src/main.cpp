#include <stdio.h>
#include "log.hpp"
#include "pin.hpp"
#include "lcd.hpp"
#include "settings.hpp"
#include "QTRSensors.hpp"

#include "server.hpp"
#include "motor.hpp"

#include "httplib.h"

#include <thread>
#include <chrono>
#include <thread>

int main()
{
    QTRSensors qtr;

    const uint8_t SensorCount = 8;
    uint16_t sensorValues[SensorCount];

    SettingsClass settings = SettingsClass();

    if (!PinClass::initialize())
    {
        LOG_ERROR("Main", "Failed to initialize pin class.");
        return EXIT_FAILURE;
    }
    LOG_INFORMATION("Main", "Initialized pin class.");

    ServerClass server = ServerClass();
    std::thread(&ServerClass::listen, server).detach();
    
    PinClass rightMotorEnabled(13);
    PinClass rightMotorA1(23);
    PinClass rightMotorA2(24);

    PinClass leftMotorEnabled(19);
    PinClass leftMotorA1(17);
    PinClass leftMotorA2(27);

    MotorClass rightMotor(rightMotorEnabled, rightMotorA1, rightMotorA2);
    MotorClass leftMotor(leftMotorEnabled, leftMotorA1, leftMotorA2);

    LOG_INFORMATION("Main", "Ir sensors");
    qtr.setTypeRC();
    qtr.setSensorPins((const uint8_t[]){3, 4, 5, 6, 7, 8, 9, 10}, SensorCount);
    qtr.setEmitterPin(2);

    LOG_INFORMATION("Main", "Starting program.");

    bool Direction = true;

    while (true)
    {
    }

    return EXIT_SUCCESS;
}