#ifdef RASPBERRY_PI

#include "pin.hpp"
#include "log.hpp"

#include <wiringPi.h>

void PinClass::setMode(PinMode mode)
{
    if (!this->isValid())
    {
        LOG_ERROR("Pin", "Pin instance %i is not valid.", this->pin);
        return;
    }

    switch (mode)
    {
    case PinMode::Input:
        pinMode(this->wiringPiPin, INPUT);
        break;
    case PinMode::Output:
        pinMode(this->wiringPiPin, OUTPUT);
        break;
    case PinMode::PwmOutput:
        pinMode(this->wiringPiPin, PWM_OUTPUT);
        break;
    }
}

void PinClass::writeDigital(DigitalState value)
{
    if (!this->isValid())
    {
        LOG_ERROR("Pin", "Pin instance %i is not valid.", this->pin);
        return;
    }

    switch (value)
    {
    case DigitalState::High:

        digitalWrite(this->wiringPiPin, HIGH);
        break;
    case DigitalState::Low:
        digitalWrite(this->wiringPiPin, LOW);
        break;
    }
}

DigitalState PinClass::readDigital() const
{
    if (!this->isValid())
    {
        LOG_ERROR("Pin", "Pin instance %i is not valid.", this->pin);
        return DigitalState::Low;
    }
    if (digitalRead(this->wiringPiPin) == HIGH)
        return DigitalState::High;

    return DigitalState::Low;
}

void PinClass::writeAnalog(unsigned int value)
{
    if (!this->isValid())
    {
        LOG_ERROR("Pin", "Pin instance %i is not valid.", this->pin);
        return;
    }
#ifdef NATIVE
    LOG_VERBOSE("Pin", "Analog write %i on pin %i (%i)", value, this->pin, this->wiringPiPin);
#endif
    pwmWrite(this->wiringPiPin, value);
}

int PinClass::readAnalog() const
{
    if (!this->isValid())
    {
        LOG_ERROR("Pin", "Pin instance %i is not valid.", this->pin);
        return 0;
    }
    return analogRead(this->wiringPiPin);
}

bool PinClass::initialize()
{
    return wiringPiSetupGpio() != -1;
}

#endif
