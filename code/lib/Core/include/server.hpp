#pragma once

#include "server.hpp"
#include "settings.hpp"

#include <functional>

class ServerClass
{
public:
    ServerClass();

    void listen();

    std::function<void(float, float)> on_gamepad_direction;
    std::function<void(SettingsClass)> on_settings_change;
};

