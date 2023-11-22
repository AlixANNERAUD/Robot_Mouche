#include "main.hpp"

#include <stdio.h>
#include "log.hpp"
#include "pin.hpp"
#include "lcd.hpp"
#include "settings.hpp"
#include "qtr.hpp"

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


    SettingsClass settings = SettingsClass();

    PinClass I2C_SDA(2), I2C_SCL(3);

    display(I2C_SDA, I2C_SCL);

    server();

    LOG_INFORMATION("Main", "Initialized pin class.");

    LiDARClass lidar(I2C_SDA, I2C_SCL);
    if (!lidar.isValid())
        return EXIT_FAILURE;

    PinClass leftMotorEnabled(19);
    PinClass leftMotorA1(17);
    PinClass leftMotorA2(27);

    PinClass rightMotorEnabled(13);
    PinClass rightMotorA1(23);
    PinClass rightMotorA2(24);

    MotorClass rightMotor(rightMotorEnabled, rightMotorA1, rightMotorA2);
    MotorClass leftMotor(leftMotorEnabled, leftMotorA1, leftMotorA2);

    LOG_INFORMATION("Main", "Ir sensors");

    LOG_INFORMATION("Main", "Starting program.");

    PinClass sensor1(18);
    PinClass sensor2(23);
    PinClass sensor3(24);

    QTRClass qtrClass(sensor1, sensor2, sensor3);

    while (true)
    {
        auto times = qtrClass.getTimesElapsed();
        printf("Sensor 1: %d\n", times[0]);
        printf("Sensor 2: %d\n", times[1]);
        printf("Sensor 3: %d\n", times[2]);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return EXIT_SUCCESS;
}