#include "driver.hpp"
#include "log.hpp"
#include <algorithm>

using namespace std;

DriverClass::DriverClass(MotorClass &left, MotorClass &right, QTRSensors &qtr, SettingsClass &settings)
    : left(left), right(right), qtr(qtr), settings(settings), pid(settings.KP, settings.KI, settings.KD, 0.0)
{
    this->running = false;
    this->speed = 0.0f;
    this->steering = 0.0f;
}

void DriverClass::run()
{
    // TODO: make this run in a thread
    this->running = true;
    while (true)
    {
        this->update();
    }
}

float DriverClass::computeLinePosition() {
    return 0.0f;
}

void DriverClass::update() {
    this->linePosition = this->computeLinePosition();
                    
    this->steering = std::clamp((float)(this->pid.getSteering(this->linePosition, clock()) / 2048.0), -1.0f, 1.0f); // NOMALIZE it from -1.0 to 1.0

    float speedLeft = std::min(this->speed - this->steering, 1.0f) * 1024.0f;
    float speedRight = std::min(this->speed - this->steering, 1.0f) * 1024.0f;

    this->left.setSpeed((unsigned int)speedLeft);
    this->right.setSpeed((unsigned int)speedRight);
}

void DriverClass::stop()
{
    this->running = false;
    this->left.setSpeed(0);
    this->right.setSpeed(0);
}
