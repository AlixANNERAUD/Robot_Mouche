#include "main.hpp"

#include "server.hpp"

#include <thread>
#include "log.hpp"
#include <functional>

void server(DriverClass &driver)
{
    ServerClass server = ServerClass();
    // Set the callbacks
    server.on_gamepad_direction = std::bind(&DriverClass::updateGamepad, &driver, std::placeholders::_1, std::placeholders::_2);
    server.on_settings_change = std::bind(&DriverClass::updateSettings, &driver, std::placeholders::_1);
    std::thread(&ServerClass::listen, server).detach();
}
