#include "server.hpp"
#include "log.hpp"
#include "httplib.h"
#include "driver.hpp"
#include <cstdlib>
#include <string>
#include "sound.hpp"

ServerClass::ServerClass()
{
    // Set default callbacks
    this->on_gamepad_direction = [](float, float)
    {
        LOG_WARNING("Server", "Unhandled gamepad direction!");
    };
    this->on_settings_change = [](SettingsClass)
    {
        LOG_WARNING("Server", "Unhandled settings change!");
    };
}

void play_sound(std::string file_name)
{
    SoundClass::play(file_name);
}

void ServerClass::listen()
{
    httplib::Server server;

    LOG_INFORMATION("Server", "Starting server");

    auto on_gamepad_direction = this->on_gamepad_direction;
    auto on_settings_change = this->on_settings_change;

    std::string Server_ressources_path = SERVER_RESSOURCES_PATH;

    LOG_INFORMATION("Server", "Serving files from %s", Server_ressources_path.c_str());

    httplib::Headers headers = {
        {"Access-Control-Allow-Origin", "*"}};

    // Serve files
    server.set_mount_point("/", Server_ressources_path, headers);

    // Set up callback for gamepad direction
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
        //LOG_DEBUG("Server", "Gamepad direction received: %f %f", x, y);

        // Call callback
        on_gamepad_direction(x, y);

        // Respond
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("All good!", "text/plain"); });

    server.Post("/settings", [on_settings_change](const httplib::Request &req, httplib::Response &res)
                { 
        // Body format: 1 byte for mode (0 = line follower, 1 = manual)

        long unsigned int expected_body_length = 1+4*8;
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
        settings.KS = *(double *)req.body.substr(25, 9).c_str();

        // Respond
        LOG_INFORMATION("Server", "Settings request received: All good!");
        LOG_INFORMATION("Server", "KP: %f, KI: %f, KD: %f, KS: %f", settings.KP, settings.KI, settings.KD, settings.KS);
        on_settings_change(settings);
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("All good!", "text/plain"); });

    // Set up callback for playing sound
    server.Post("/play-sound", [](const httplib::Request &req, httplib::Response &res)
                {
        play_sound(req.body);
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content("Playing!", "text/plain"); });

    // Set up callback for reading line position
    server.Get("/info", [this](const httplib::Request &req, httplib::Response &res)
               {
        std::array<char, 640> values = DriverClass::readLinePositionFile();
        std::array<bool, 5> line_position = DriverClass::computeLinePosition(values);

        std::array<char, 645> body;
        memcpy(body.data(), values.data(), 640);
        memcpy(body.data() + 640, &line_position, 5);

        if (sizeof(bool) != 1) {
            LOG_ERROR("Server", "sizeof(bool) != 1");
        }

        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(body.data(), body.size(), "application/octet-stream"); });

    server.listen("0.0.0.0", 80);
    LOG_INFORMATION("Server", "Server stopped");
}
