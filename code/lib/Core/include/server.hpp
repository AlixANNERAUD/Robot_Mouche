#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

#include "server.hpp"
#include "settings.hpp"

#include <functional>

typedef void GamepadDirectionCallback(float, float);
typedef void SettingsChangeCallback(SettingsClass);

class ServerClass
{
public:
    ServerClass();

    void listen();

    std::function<void(float, float)> on_gamepad_direction;
    std::function<void(SettingsClass)> on_settings_change;
};

#endif // SERVER_HPP_INCLUDED
