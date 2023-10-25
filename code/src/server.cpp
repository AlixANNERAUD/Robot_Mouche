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
    httplib::Server server;

    LOG_INFORMATION("Server", "Starting server");

    GamepadDirectionCallback *on_gamepad_direction = this->on_gamepad_direction;

    server.Post("/gamepad-direction", [on_gamepad_direction](const httplib::Request &req, httplib::Response &res)
    { 
        // Ensure body is long enough
        if (req.body.length() < 8) {
            LOG_ERROR("Server", "Gamepad direction request body too short!");
            res.set_content("Gamepad direction request body too short!", "text/plain"); 
            return;
        }

        // Read game pad direction from body which is 4 bytes for x and 4 bytes for y as LE
        float x = *(float *)req.body.substr(0, 4).c_str();
        float y = *(float *)req.body.substr(4, 4).c_str();
        LOG_DEBUG("Server", "Gamepad direction received: %f %f", x, y);

        // Call callback
        on_gamepad_direction(x, y);

        // Respond
        res.set_content("All good!", "text/plain"); 
    });

    server.Post("/settings", [](const httplib::Request &req, httplib::Response &res)
    { 
        // Body format: 1 byte for mode (0 = line follower, 1 = manual)

        if (req.body.length() != 1) {
            LOG_ERROR("Server", "Settings request body wrong length!");
            res.set_content("Settings request body wrong length!", "text/plain");
            return;
        }

        // Read body
        switch (req.body[0]) {
            case 0:
                LOG_DEBUG("Server", "Settings request received: Line follower");
                break;
            case 1:
                LOG_DEBUG("Server", "Settings request received: Manual");
                break;
            default:
                LOG_ERROR("Server", "Settings request received: Unknown mode");
                res.set_content("Settings request received: Unknown mode", "text/plain");
                return;
        }

        // Respond
        res.set_content("All good!", "text/plain"); 
    });

    server.listen("0.0.0.0", 8080);
}
