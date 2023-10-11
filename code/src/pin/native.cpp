#ifdef NATIVE

#include "pin.hpp"

#include "log.hpp"

void PinClass::setMode(PinMode mode)
{
    if (!this->isValid())
    {
        LOG_ERROR("Pin", "Pin instance %i is not valid.", this->pin);
        return;
    }
    this->mode = mode;
}

void PinClass::writeDigital(DigitalState value)
{
    if (!this->isValid())
    {
        LOG_ERROR("Pin", "Pin instance %i is not valid.", this->pin);
        return;
    }
    this->digital_value = value;
}

DigitalState PinClass::readDigital() const
{
    if (!this->isValid())
    {
        LOG_ERROR("Pin", "Pin instance %i is not valid.", this->pin);
        return DigitalState::Low;
    }
    return this->digital_value;
}

void PinClass::writeAnalog(unsigned int value)
{
    if (!this->isValid())
    {
        LOG_ERROR("Pin", "Pin instance %i is not valid.", this->pin);
        return;
    }
    this->analog_value = value;
}

int PinClass::readAnalog() const
{
    if (!this->isValid())
    {
        LOG_ERROR("Pin", "Pin instance %i is not valid.", this->pin);
        return 0;
    }
    return this->analog_value;
}

bool PinClass::initialize()
{
    LOG_INFORMATION("Pin", "Initialized pin abstraction layer.");
    return true;
}

#endif
