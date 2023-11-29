#include "settings.hpp"

SettingsClass::SettingsClass()
{
    this->mode = RobotMode::Manual;
    this->KP = 1.0;
    this->KI = 1.0;
    this->KD = 1.0;
    this->irs_threshold = 50;
}

SettingsClass::SettingsClass(RobotMode mode)
{
    this->mode = mode;
    this->KP = 1.0;
    this->KI = 1.0;
    this->KD = 1.0;
    this->irs_threshold = 50;
}
