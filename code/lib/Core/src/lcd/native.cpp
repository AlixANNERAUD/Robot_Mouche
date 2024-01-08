#ifdef NATIVE

#include "lcd.hpp"

#include "log.hpp"

#include <stdarg.h>

LCDClass::LCDClass(PinClass& SDA, PinClass& SCL) : SDA(SDA), SCL(SCL), valid(true)
{
    if (!SDA.isValid() || !SCL.isValid())
    {
        LOG_ERROR("LCD", "SDA or SCL pin is not valid.");
        this->valid = false;
        return;
    }
}

void LCDClass::setCursor(unsigned int x, unsigned int y)
{
    if (!this->isValid())
    {
        LOG_ERROR("LCD", "LCD instance is not valid.");
        return;
    }

    this->x = x;
    this->y = y;
}

void LCDClass::printFormatted(const char* format, ...)
{
    if (!this->isValid())
    {
        LOG_ERROR("LCD", "LCD instance is not valid.");
        return;
    }
    va_list args;
    va_start(args, format);
    snprintf(this->buffer, sizeof(buffer), format, args);
    va_end(args);
}

#endif
