#include "main.hpp"

#include "server.hpp"

#include <thread>
#include "log.hpp"
#include <functional>

void server(DriverClass &driver)
{
    ServerClass server = ServerClass();
    auto callback = std::bind(&DriverClass::updatePidConstants, &driver, std::placeholders::_1);
    server.on_settings_change = callback;
    std::thread(&ServerClass::listen, server).detach();
}
