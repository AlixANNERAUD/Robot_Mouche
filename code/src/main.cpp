#include <stdio.h>
#include "log.hpp"
#include "pin.hpp"
#include "lcd.hpp"

int main()
{
    if (!PinClass::initialize())
    {
        LOG_ERROR("Main", "Failed to initialize pin class.");
        return 1;
    }


    LOG_INFORMATION("Main", "Starting program.");

    return 0;
}