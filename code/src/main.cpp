#include "main.hpp"

#include <stdio.h>
#include "log.hpp"
#include "pin.hpp"
#include "lcd.hpp"
#include "driver.hpp"

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

    PinClass I2C_SDA(2), I2C_SCL(3);

    // display(I2C_SDA, I2C_SCL);

    LOG_INFORMATION("Main", "Initialized pin class.");

    LiDARClass lidar(I2C_SDA, I2C_SCL);
    if (!lidar.isValid())
        return EXIT_FAILURE;

    PinClass rightMotorEnabled(13);
    PinClass rightMotorA1(27);
    PinClass rightMotorA2(17);

    PinClass leftMotorEnabled(12);
    PinClass leftMotorA1(24);
    PinClass leftMotorA2(23);

    MotorClass rightMotor(rightMotorEnabled, rightMotorA1, rightMotorA2);
    MotorClass leftMotor(leftMotorEnabled, leftMotorA1, leftMotorA2);

    LOG_INFORMATION("Main", "Starting program.");

    DriverClass driver(lidar, leftMotor, rightMotor);

    server(driver);

    driver.start();
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    //    driver.stop();
    return EXIT_SUCCESS;
}
