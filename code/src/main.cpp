#include "main.hpp"

#include <stdio.h>
#include "log.hpp"
#include "pin.hpp"
#include "lcd.hpp"
#include "settings.hpp"
#include "QTRSensors.hpp"

#include "motor.hpp"
#include "lidar.hpp"

#include "httplib.h"

#include <thread>
#include <chrono>
#include <thread>

int main()
{
    if (!PinClass::initialize())
    {
        LOG_ERROR("Main", "Failed to initialize pin class.");
        return EXIT_FAILURE;
    }

    //QTRSensors qtr;

    //const uint8_t SensorCount = 8;
    //uint16_t sensorValues[SensorCount];

    SettingsClass settings = SettingsClass();

    PinClass I2C_SDA(2), I2C_SCL(3);

    display(I2C_SDA, I2C_SCL);

    server();

    LOG_INFORMATION("Main", "Initialized pin class.");

    LiDARClass lidar(I2C_SDA, I2C_SCL);
    if (!lidar.isValid())
        return EXIT_FAILURE;

    while (true)
    {
        LOG_INFORMATION("Main", "Amplitude: %d", lidar.getAmplitude());
        LOG_INFORMATION("Main", "Distance: %d", lidar.getDistance());
        LOG_INFORMATION("Main", "Error: %d", lidar.getError());
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }


    PinClass leftMotorEnabled(19);
    PinClass leftMotorA1(17);
    PinClass leftMotorA2(27);

    PinClass rightMotorEnabled(13);
    PinClass rightMotorA1(23);
    PinClass rightMotorA2(24);

    MotorClass rightMotor(rightMotorEnabled, rightMotorA1, rightMotorA2);
    MotorClass leftMotor(leftMotorEnabled, leftMotorA1, leftMotorA2);

    LOG_INFORMATION("Main", "Ir sensors");

    //qtr.setTypeRC();
    //qtr.setSensorPins((const uint8_t[]){3, 4, 5, 6, 7, 8, 9, 10}, SensorCount);
    //qtr.setEmitterPin(2);

    LOG_INFORMATION("Main", "Starting program.");

    while (true)
    {
    }

    return EXIT_SUCCESS;
}