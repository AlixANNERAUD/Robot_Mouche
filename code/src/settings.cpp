#include "settings.hpp"

SettingsClass::SettingsClass()
{
    this->mode = RobotMode::Manual;
}

SettingsClass::SettingsClass(RobotMode mode)
{
    this->mode = mode;
}
