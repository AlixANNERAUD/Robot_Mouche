#include "lcd.hpp"

#include "log.hpp"

#include <thread>

#include <cstdarg>

#include <wiringPi.h>
#include <lcd.h>

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

void LCDClass::printFormatted(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    int i = snprintf(NULL, 0, format, args);
    char buffer[i + 1];
    snprintf(buffer, sizeof(buffer), format, args);
    this->print(buffer);
    va_end(args);
}