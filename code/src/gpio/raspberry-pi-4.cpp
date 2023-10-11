#ifdef RASPBERRY_PI_4

#include "abstraction.hpp"
#include "stdio.h"

bool GpioClass::initialize()
{    
    return wiringPiSetup() != -1;
}

void GpioClass::pinMode(int pin, int mode)
{
    pinMode(pin, mode);
}



#endif
