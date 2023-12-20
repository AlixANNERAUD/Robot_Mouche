#include "server.hpp"
#include "log.hpp"
#include "httplib.h"
#include "driver.hpp"
#include <cstdlib>
#include <string>

ServerClass::ServerClass() {
    this->on_gamepad_direction = [](float, float) {
        LOG_WARNING("Server", "Unhandled gamepad direction!");
    };
    this->on_settings_change = [](SettingsClass) {
        LOG_WARNING("Server", "Unhandled settings change!");
    };
}

void play_sound(std::string file_name) {
    LOG_INFORMATION("Server", "Playing sound: %s", file_name.c_str());
    system("pkill -9 mpg321");
    system("amixer -q set PCM,0 unmute");
    std::string command = "mpg321 -q assets/" + file_name + " &";
    system(command.c_str());
}

void ServerClass::listen() {
    httplib::Server server;

    LOG_INFORMATION("Server", "Starting server");

    auto on_gamepad_direction = this->on_gamepad_direction;
    auto on_settings_change = this->on_settings_change;

    std::string Server_ressources_path = SERVER_RESSOURCES_PATH;

    LOG_INFORMATION("Server", "Serving files from %s", Server_ressources_path.c_str());

    httplib::Headers headers = {
        {"Access-Control-Allow-Origin", "*"}
    };

    server.set_mount_point("/", Server_ressources_path, headers);

    server.Post("/gamepad-direction", [on_gamepad_direction](const httplib::Request &req, httplib::Response &res)
    { 
        // Ensure body is the right length
        if (req.body.length() != 8) {
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

    server.Post("/play-sound", [](const httplib::Request &req, httplib::Response &res){
        play_sound(req.body);
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("Playing!", "text/plain");
    });

    server.Get("/info", [this](const httplib::Request &req, httplib::Response &res)
    {
        std::array<char, 640> values = DriverClass::readLinePositionFile();
        double line_position = DriverClass::computeLinePosition(values);

        std::array<char, 648> body;
        memcpy(body.data(), values.data(), 640);
        memcpy(body.data() + 640, &line_position, 8);

        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(body.data(), body.size(), "application/octet-stream");
    });

    server.listen("0.0.0.0", 80);
    LOG_INFORMATION("Server", "Server stopped");
}
