#ifdef RASPBERRY_PI

#include "lcd.hpp"

#include "log.hpp"

#include <wiringPi.h>
#include <pcf8574.h>
#include <lcd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <cstdarg>
#include <cstring>

#define AF_BASE 64
#define AF_RS (AF_BASE + 0)
#define AF_RW (AF_BASE + 1)
#define AF_E (AF_BASE + 2)
#define AF_LED (AF_BASE + 3)

#define AF_DB4 (AF_BASE + 4)
#define AF_DB5 (AF_BASE + 5)
#define AF_DB6 (AF_BASE + 6)
#define AF_DB7 (AF_BASE + 7)

LCDClass::LCDClass(PinClass& SDA, PinClass& SCL) : valid(true), SDA(SDA), SCL(SCL)
{
    if (!SDA.isValid() || !SCL.isValid())
    {
        LOG_ERROR("LCD", "SDA or SCL pin is not valid.");
        this->valid = false;
        return;
    }

    if (pcf8574Setup(AF_BASE, 0x27) == -1)
    {
        LOG_ERROR("LCD", "Failed to setup PCF8574.");
        this->valid = false;
        return;
    }

    this->handle = lcdInit(2, 16, 4, AF_RS, AF_E, AF_DB4, AF_DB5, AF_DB6, AF_DB7, 0, 0, 0, 0);

    if (this->handle < 0)
    {
        LOG_ERROR("LCD", "Failed to initialize LCD.");
        this->valid = false;
        return;
    }

    // Set all pins to output on the PCF8574
    for (int i = 0; i < 8; i++)
        pinMode(AF_BASE + i, OUTPUT);
    

    memset(this->content, ' ', sizeof(this->content));
    memset(this->newContent, ' ', sizeof(this->newContent));


    digitalWrite(AF_LED, 1);
    digitalWrite(AF_RW, 0);

    this->clear();
}

void LCDClass::setCursor(unsigned int x, unsigned int y)
{
    lcdPosition(this->handle, x, y);
}



#endif