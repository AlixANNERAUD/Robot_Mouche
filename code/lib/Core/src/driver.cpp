#include "driver.hpp"
#include "log.hpp"
#include <algorithm>

using namespace std;

DriverClass::DriverClass(MotorClass &left, MotorClass &right, QTRSensors &qtr, SettingsClass &settings)
    : left(left), right(right), qtr(qtr), settings(settings)
{
    this->running = false;
    this->speed = 0.0f;
    this->steering = 0.0f;
    this->pid = PidControlClass(settings.KP, settings.KI, settings.KD, 0.0);
}

void DriverClass::run()
{
    this->running = true;
    while (this->running)
    {
        this->linePosition = this->computeLinePosition();
                        
        this->steering = (float)this->pid.getSteering(this->linePosition, clock()); // NOMALIZE it from -1.0 to 1.0

        float speedLeft = std::min(this->speed - this->steering, 1.0f) * 1024.0f;
        float speedRight = std::min(this->speed - this->steering, 1.0f) * 1024.0f;

        this->left.setSpeed((unsigned int)speedLeft);
        this->right.setSpeed((unsigned int)speedRight);
    }
}

void DriverClass::stop()
{
    this->running = false;
}

