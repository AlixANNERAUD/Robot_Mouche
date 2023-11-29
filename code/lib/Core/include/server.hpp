#ifndef SERVER_HPP_INCLUDED
#define SERVER_HPP_INCLUDED

#include "server.hpp"
#include "qtr.hpp"
#include "settings.hpp"

#include <functional>

class ServerClass
{
public:
    ServerClass(QTRClass *qtr1, QTRClass *qtr2);

    void listen();

    QTRClass *qtr1;
    QTRClass *qtr2;
    std::function<void(float, float)> on_gamepad_direction;
    std::function<void(SettingsClass)> on_settings_change;
};

#endif // SERVER_HPP_INCLUDED
