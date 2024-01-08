#include "lcd.hpp"
#include "log.hpp"
#include <thread>
#include <cstdarg>

#include <cstring>

#ifdef RASPBERRY_PI
#include <wiringPi.h>
#include <lcd.h>
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

void LCDClass::print(char text)
{
#ifdef RASPBERRY_PI
    lcdPutchar(this->handle, text);
#endif
#ifdef NATIVE
    LOG_INFORMATION("LCD", "Printed \"%c\" to LCD.", text);
#endif
}


