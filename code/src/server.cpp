#include "server.hpp"
#include "log.hpp"
#include "httplib.h"

ServerClass::ServerClass() {
    this->on_gamepad_direction = [](float, float) {
        LOG_WARNING("Server", "Unhandled gamepad direction!");
    };
    this->on_settings_change = [](float, float) {
        LOG_WARNING("Server", "Unhandled settings change!");
    };
}

void ServerClass::listen() {
    httplib::Server svr;

    LOG_INFORMATION("Server", "Starting server");

    svr.Get("/hi", [](const httplib::Request &req, httplib::Response &res)
    { 
                res.set_content("Hello World!", "text/plain"); 
    });

    svr.listen("0.0.0.0", 8080);
}
