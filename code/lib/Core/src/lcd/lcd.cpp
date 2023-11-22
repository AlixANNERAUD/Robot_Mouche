#include "lcd.hpp"
#include "log.hpp"
#include <thread>
#include <cstdarg>

#ifdef RASPBERRY_PI
    #include <wiringPi.h>
    #include <lcd.h>
#endif

bool LCDClass::isValid() const
{
    return this->valid;
}

void LCDClass::print(const char* text)
{
#ifdef RASPBERRY_PI
    lcdPuts(this->handle, text);
#endif
    LOG_INFORMATION("LCD", "Printed \"%s\" to LCD.", text);
}
