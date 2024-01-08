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
    LOG_INFORMATION("Main", "Starting program.");

    // Initialize the pin class
    if (!PinClass::initialize())
    {
        LOG_ERROR("Main", "Failed to initialize pin class.");
        return EXIT_FAILURE;
    }
    LOG_INFORMATION("Main", "Initialized pin class.");

    PinClass I2C_SDA(2), I2C_SCL(3);

    LiDARClass lidar(I2C_SDA, I2C_SCL);
    if (!lidar.isValid())
        return EXIT_FAILURE;

    LCDClass lcd(I2C_SDA, I2C_SCL);
    if (!lcd.isValid()) {
        return EXIT_FAILURE;
    }

    PinClass rightMotorEnabled(13);
    PinClass rightMotorA1(27);
    PinClass rightMotorA2(17);

    PinClass leftMotorEnabled(12);
    PinClass leftMotorA1(24);
    PinClass leftMotorA2(23);

    MotorClass rightMotor(rightMotorEnabled, rightMotorA1, rightMotorA2);
    MotorClass leftMotor(leftMotorEnabled, leftMotorA1, leftMotorA2);

    DriverClass driver(lidar, leftMotor, rightMotor, lcd);

    // Start the web server
    server(driver);

    // Start the driver
    driver.start();

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    driver.stop();
    return EXIT_SUCCESS;
}
