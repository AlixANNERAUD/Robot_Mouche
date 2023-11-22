#include "server.hpp"
#include "log.hpp"
#include "httplib.h"

ServerClass::ServerClass() {
    this->on_gamepad_direction = [](float, float) {
        LOG_WARNING("Server", "Unhandled gamepad direction!");
    };
    this->on_settings_change = [](SettingsClass) {
        LOG_WARNING("Server", "Unhandled settings change!");
    };
}

void ServerClass::listen() {
    httplib::Server server;

    LOG_INFORMATION("Server", "Starting server");

    GamepadDirectionCallback *on_gamepad_direction = this->on_gamepad_direction;
    SettingsChangeCallback *on_settings_change = this->on_settings_change;

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
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("All good!", "text/plain"); 
    });

    server.Post("/settings", [on_settings_change](const httplib::Request &req, httplib::Response &res)
    { 
        // Body format: 1 byte for mode (0 = line follower, 1 = manual)

        long unsigned int expected_body_length = 1+3*8+2;
        if (req.body.length() != expected_body_length) {
            LOG_ERROR("Server", "Settings request body wrong length! (got %d, expected %d)", req.body.length(), expected_body_length);
            res.set_content("Settings request body wrong length!", "text/plain");
            return;
        }

        // Read body
        SettingsClass settings = SettingsClass();
        switch (req.body[0]) {
            case 0:
                LOG_DEBUG("Server", "Settings request received: Line follower");
                settings.mode = RobotMode::LineFollower;
                break;
            case 1:
                LOG_DEBUG("Server", "Settings request received: Manual");
                settings.mode = RobotMode::Manual;
                break;
            default:
                LOG_ERROR("Server", "Settings request received: Unknown mode");
                res.set_content("Settings request received: Unknown mode", "text/plain");
                return;
        }
        settings.KP = *(double *)req.body.substr(1, 9).c_str();
        settings.KI = *(double *)req.body.substr(9, 9).c_str();
        settings.KD = *(double *)req.body.substr(17, 9).c_str();
        settings.irs_threshold = *(short int *)req.body.substr(25, 2).c_str();

        // Respond
        LOG_INFORMATION("Server", "Settings request received: All good!");
        LOG_INFORMATION("Server", "KP: %f, KI: %f, KD: %f", settings.KP, settings.KI, settings.KD);
        LOG_INFORMATION("Server", "IRS threshold: %d", settings.irs_threshold);
        on_settings_change(settings);
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("All good!", "text/plain");
    });

    server.listen("0.0.0.0", 8080);
}
