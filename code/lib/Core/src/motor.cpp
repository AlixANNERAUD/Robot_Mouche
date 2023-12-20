#include "motor.hpp"

#include "log.hpp"

#ifdef RASPBERRY_PI
    #include "wiringPi.h"
#endif

#include <string>
#include <algorithm>

MotorClass::MotorClass(PinClass &Enabled, PinClass &A1, PinClass &A2) : valid(false),
                                                                        Enabled(Enabled),
                                                                        A1(A1),
                                                                        A2(A2)
{
    if (!Enabled.isValid() || !A1.isValid() || !A2.isValid())
    {
        LOG_ERROR("Motor", "Pins are is not valid.");
        return;
    }

    LOG_DEBUG("Motor", "Motor instance created.");

    this->valid = true;

    Enabled.setMode(PinMode::PwmOutput);
    A1.setMode(PinMode::Output);
    A2.setMode(PinMode::Output);

#ifdef RASPBERRY_PI
    pwmSetMode(PWM_MODE_MS);
    pwmSetClock(384);
#endif
}

void MotorClass::setSpeed(unsigned int speed)
{
    if (!this->isValid())
    {
        LOG_ERROR("Motor", "Motor instance is not valid.");
        return;
    }
    LOG_INFORMATION("Motor", "Speed : %u", speed);

    this->Enabled.writeAnalog(speed);
#ifdef NATIVE
    this->speed = speed;
#endif
}

void MotorClass::setDirection(MotorDirection direction)
{
    if (!this->isValid())
    {
        LOG_ERROR("Motor", "Motor instance is not valid.");
        return;
    }

    switch (direction)
    {
    case MotorDirection::Forward:
        LOG_DEBUG("Motor", "Direction : Forward");
        this->A1.writeDigital(DigitalState::High);
        this->A2.writeDigital(DigitalState::Low);
        break;
    case MotorDirection::Backward:
        LOG_DEBUG("Motor", "Direction : Backward");
        this->A1.writeDigital(DigitalState::Low);
        this->A2.writeDigital(DigitalState::High);
        break;
    }

#ifdef NATIVE
    this->direction = direction;
#endif
}

void MotorClass::set(float relativeSpeed)
{
    if (!this->isValid())
    {
        LOG_ERROR("Motor", "Motor instance is not valid.");
        return;
    }

    relativeSpeed = std::clamp(relativeSpeed, -1.0f, 1.0f);

    LOG_DEBUG("Motor", "Set direction and speed.");
    this->setDirection(direction);
    this->setSpeed(speed);
}

void MotorClass::stop()
{
    if (!this->isValid())
    {
        LOG_ERROR("Motor", "Motor instance is not valid.");
        return;
    }

    LOG_DEBUG("Motor", "Stop motor.");

    this->Enabled.writeAnalog(0);
}

bool MotorClass::isValid() const
{
    return this->valid;
}