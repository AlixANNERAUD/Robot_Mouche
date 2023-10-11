#ifdef RASPBERRY_PI_4

#include "pin.hpp"

#include <wiringPi.h>

void PinClass::setMode(PinMode mode)
{
    if (!this->isValid())
    {
        LOG_ERROR("Pin", "Pin instance %i is not valid.", this->pin);
        return;
    }    
    pinMode(this->pin, (int)mode);
}

void PinClass::writeDigital(DigitalState value)
{
    if (!this->isValid())
    {
        LOG_ERROR("Pin", "Pin instance %i is not valid.", this->pin);
        return;
    }
    digitalWrite(this->pin, (int)value);
}

DigitalState PinClass::readDigital() const
{
    if (!this->isValid())
    {
        LOG_ERROR("Pin", "Pin instance %i is not valid.", this->pin);
        return DigitalState::Low;
    }
    switch (digitalRead(this->pin))
    {
    case HIGH:
        return DigitalState::High;
    case LOW:
        return DigitalState::Low;
    }
}

void PinClass::writeAnalog(unsigned int value)
{
    if (!this->isValid())
    {
        LOG_ERROR("Pin", "Pin instance %i is not valid.", this->pin);
        return;
    }
    analogWrite(this->pin, value);
}

int PinClass::readAnalog() const
{
    if (!this->isValid())
    {
        LOG_ERROR("Pin", "Pin instance %i is not valid.", this->pin);
        return 0;
    }
    return analogRead(this->pin);
}

bool PinClass::initialize()
{
    if (wiringPiSetup() == -1)
        return false;
        
    return true;
}

#endif
