#ifdef RASPBERRY_PI_4

#include "motor.hpp"

#include "log.hpp"

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

    this->valid = true;

    Enabled.setMode(PinMode::PwmOutput);
    A1.setMode(PinMode::Output);
    A2.setMode(PinMode::Output);

    pwmSetMode(PWM_MODE_MS);
    pwmSetClock(192);
}

void MotorClass::setSpeed(unsigned int speed)
{
    if (!this->isValid())
    {
        LOG_ERROR("Motor", "Motor instance is not valid.");
        return;
    }

    this->Enabled.writeAnalog(speed);
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
        this->A1.writeDigital(DigitalState::High);
        this->A2.writeDigital(DigitalState::Low);
        break;
    case MotorDirection::Backward:
        this->A1.writeDigital(DigitalState::Low);
        this->A2.writeDigital(DigitalState::High);
        break;
    }
}

void MotorClass::set(MotorDirection direction, unsigned int speed)
{
    if (!this->isValid())
    {
        LOG_ERROR("Motor", "Motor instance is not valid.");
        return;
    }

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

    this->Enabled.writeAnalog(0);
}

#endif