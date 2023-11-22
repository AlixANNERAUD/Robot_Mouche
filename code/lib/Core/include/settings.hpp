#ifndef SETTINGS_HPP_INCLUDED
#define SETTINGS_HPP_INCLUDED

#include "settings.hpp"

enum class RobotMode
{
    LineFollower,
    Manual,
};

class SettingsClass
{
public:
    SettingsClass();
    SettingsClass(RobotMode mode);

    RobotMode mode;
    double KP, KI, KD;
};

#endif // SETTINGS_HPP_INCLUDED
