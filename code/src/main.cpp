#include <stdio.h>
#include "log.hpp"
#include "pin.hpp"
#include "lcd.hpp"
#include "settings.hpp"
#include "server.hpp"

#include <thread>

#include "httplib.h"


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

    PinClass Pin(2);

    Pin.setMode(PinMode::Output);

    LOG_INFORMATION("Main", "Starting program.");

    while (true)
    {
        
    }
    

    return EXIT_SUCCESS;
}