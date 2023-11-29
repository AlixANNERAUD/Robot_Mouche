#include "main.hpp"

#include "server.hpp"
#include "qtr.hpp"

#include <thread>
#include "log.hpp"
#include <functional>

void server(QTRClass *qtr1, QTRClass *qtr2, DriverClass &driver)
{
    ServerClass server = ServerClass(qtr1, qtr2);
    auto callback = std::bind(&DriverClass::updatePidConstants, &driver, std::placeholders::_1);
    server.on_settings_change = callback;
    std::thread(&ServerClass::listen, server).detach();
}
