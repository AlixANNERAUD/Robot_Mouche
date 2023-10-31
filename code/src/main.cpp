#include <stdio.h>
#include "log.hpp"
#include "pin.hpp"
#include "lcd.hpp"
#include "settings.hpp"
#include "server.hpp"
#include "motor.hpp"

#include "httplib.h"

#include <thread>
#include <chrono>
#include <thread>


int main()
{
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

    LOG_INFORMATION("Main", "Starting program.");

    bool Direction = true;

    while (true)
    {
        if (Direction == true) {
            rightMotor.setSpeed(1024);
            leftMotor.setSpeed(1024);
            rightMotor.setDirection(MotorDirection::Forward);
            leftMotor.setDirection(MotorDirection::Forward);
            Direction = false;
        }
        else {
            rightMotor.setSpeed(1024);
            leftMotor.setSpeed(1024);
            rightMotor.setDirection(MotorDirection::Backward);
            leftMotor.setDirection(MotorDirection::Backward);
            Direction = true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

        
        
    }

    return EXIT_SUCCESS;
}