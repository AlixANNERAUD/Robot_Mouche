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
    LOG_DEBUG("Pin", "Setting pin %i to mode %i.", this->pin, static_cast<int>(mode));
    this->mode = mode;
}

void PinClass::writeDigital(DigitalState value)
{
    if (!this->isValid())
    {
        LOG_ERROR("Pin", "Pin instance %i is not valid.", this->pin);
        return;
    }
    LOG_DEBUG("Pin", "Writing digital value %i to pin %i.", static_cast<int>(value), this->pin);
    this->digital_value = value;
}

DigitalState PinClass::readDigital() const
{
    if (!this->isValid())
    {
        LOG_ERROR("Pin", "Pin instance %i is not valid.", this->pin);
        return DigitalState::Low;
    }
    LOG_DEBUG("Pin", "Reading digital value from pin %i.", this->pin);
    return this->digital_value;
}

void PinClass::writeAnalog(unsigned int value)
{
    if (!this->isValid())
    {
        LOG_ERROR("Pin", "Pin instance %i is not valid.", this->pin);
        return;
    }
    LOG_DEBUG("Pin", "Writing analog value %i to pin %i.", value, this->pin);
    this->analog_value = value;
}

int PinClass::readAnalog() const
{
    if (!this->isValid())
    {
        LOG_ERROR("Pin", "Pin instance %i is not valid.", this->pin);
        return 0;
    }
    LOG_DEBUG("Pin", "Reading analog value from pin %i.", this->pin);
    return this->analog_value;
}

bool PinClass::initialize()
{
    LOG_INFORMATION("Pin", "Initialized pin abstraction layer.");
    return true;
}

#endif
