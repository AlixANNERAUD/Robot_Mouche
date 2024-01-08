#include "lcd.hpp"
#include "log.hpp"
#include <thread>
#include <cstdarg>

#include <cstring>

#ifdef RASPBERRY_PI
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
#endif

bool LCDClass::isValid() const
{
    return this->valid;
}

void LCDClass::update()
{
    // Update the content
    for (uint8_t i = 0; i < 2; i++)
    {
        for (uint8_t j = 0; j < 16; j++)
        {
            if (this->content[i][j] != this->newContent[i][j])
            {
                if (this->newContent[i][j] == '\0')
                    this->newContent[i][j] = ' ';
                this->content[i][j] = this->newContent[i][j];
                this->setCursor(j, i);
                this->print(this->newContent[i][j]);
            }
        }
    }
}

void LCDClass::print(unsigned int line, const char *text)
{
    // Set the new content
    memset(this->newContent[line], ' ', sizeof(this->newContent[0]));
    strncpy(this->newContent[line], text, sizeof(this->newContent[0]));
    this->update();
}

void LCDClass::printFormatted(unsigned int line, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    memset(this->newContent[line], ' ', sizeof(this->newContent[0]));
    vsnprintf(this->newContent[line], sizeof(this->newContent[0]), format, args);
    this->update();
    va_end(args);
}

void LCDClass::clear()
{
    memset(this->content, ' ', sizeof(this->content));
    memset(this->newContent, ' ', sizeof(this->newContent));
#ifdef RASPBERRY_PI
    lcdClear(this->handle);
#endif
#ifdef NATIVE
    if (!this->isValid())
    {
        LOG_ERROR("LCD", "LCD instance is not valid.");
        return;
    }

    LOG_INFORMATION("LCD", "Cleared LCD.");
#endif
}

LCDClass::LCDClass(PinClass &SDA, PinClass &SCL) :  valid(false), SDA(SDA), SCL(SCL)
{
    if (!SDA.isValid() || !SCL.isValid())
    {
        LOG_ERROR("LCD", "SDA or SCL pin is not valid.");
        return;
    }

#ifdef RASPBERRY_PI
    if (pcf8574Setup(AF_BASE, 0x27) == -1)
    {
        LOG_ERROR("LCD", "Failed to setup PCF8574.");
        return;
    }

    this->handle = lcdInit(2, 16, 4, AF_RS, AF_E, AF_DB4, AF_DB5, AF_DB6, AF_DB7, 0, 0, 0, 0);

    if (this->handle < 0)
    {
        LOG_ERROR("LCD", "Failed to initialize LCD.");
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
#endif
#ifdef NATIVE
    LOG_INFORMATION("LCD", "Initialized LCD.");
#endif
    this->valid = true;
}

void LCDClass::print(char text)
{
#ifdef RASPBERRY_PI
    lcdPutchar(this->handle, text);
#endif
#ifdef NATIVE
    LOG_INFORMATION("LCD", "Printed \"%c\" to LCD.", text);
#endif
}

void LCDClass::setCursor(unsigned int x, unsigned int y)
{
#ifdef RASPBERRY_PI
    lcdPosition(this->handle, x, y);
#endif
#ifdef NATIVE
    LOG_INFORMATION("LCD", "Set cursor to (%d, %d).", x, y);
#endif
}
