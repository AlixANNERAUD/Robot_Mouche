#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

#include "server.hpp"
#include "qtr.hpp"
#include "settings.hpp"

typedef void GamepadDirectionCallback(float, float);
typedef void SettingsChangeCallback(SettingsClass);

class ServerClass
{
public:
    ServerClass(QTRClass *qtr1, QTRClass *qtr2);

    void listen();

    GamepadDirectionCallback *on_gamepad_direction;
    SettingsChangeCallback *on_settings_change;
    QTRClass *qtr1;
    QTRClass *qtr2;
};

#endif // SERVER_HPP_INCLUDED
