#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

#include "server.hpp"
#include "settings.hpp"

typedef void GamepadDirectionCallback(float, float);
typedef void SettingsChangeCallback(float, float);

class ServerClass
{
public:
    ServerClass();

    void listen();

    GamepadDirectionCallback *on_gamepad_direction;
    SettingsChangeCallback *on_settings_change;
};

#endif // SERVER_HPP_INCLUDED
