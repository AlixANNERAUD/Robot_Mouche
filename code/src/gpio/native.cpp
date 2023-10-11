#ifdef NATIVE

#include "abstraction.hpp"

#include "log.hpp"

#include "stdio.h"

bool GpioClass::initialize()
{
    this->initialized = true;
    LOG_INFORMATION("GPIO", "GPIO initialized.");
    return true;
}

void GpioClass::pinMode(int pin, int mode)
{
    this->pins[pin].mode = mode;
    LOG_INFORMATION("GPIO", "Set pin %i to mode %i.", pin, mode);
}

void GpioClass::digitalWrite(int pin, int value)
{
    this->pins[pin].value = value;
    LOG_INFORMATION("GPIO", "Write pin %i to digital state %i", pin, value);
}

int GpioClass::digitalRead(int pin)
{
    LOG_INFORMATION("GPIO", "Read pin %i to digital state %i", pin, this->pins[pin].value);
    return this->pins[pin].value;
}

void GpioClass::analogWrite(int pin, int value)
{
    this->pins[pin].value = value;
    LOG_INFORMATION("GPIO", "Write pin %i to analog state %i", pin, value);
}

int GpioClass::analogRead(int pin)
{
    LOG_INFORMATION("GPIO", "Read pin %i to analog state %i", pin, this->pins[pin].value);
    return this->pins[pin].value;
}





#endif
