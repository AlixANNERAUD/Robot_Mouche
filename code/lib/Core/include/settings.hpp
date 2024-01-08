#pragma once

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
    double KP, KI, KD, KS;
    short int irs_threshold;
};