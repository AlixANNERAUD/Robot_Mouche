#ifndef DISPLAY_HPP_INCLUDED
#define DISPLAY_HPP_INCLUDED

#include "pin.hpp"

#include <atomic>

#include <string>

class LCDClass
{
public:
    LCDClass() = delete;
    LCDClass(PinClass& SDA, PinClass& SCL);
    LCDClass(const LCDClass&) = delete;
    LCDClass& operator=(const LCDClass&) = delete;

    void clear();
    void print(unsigned int line, const char* text);
    void printFormatted(unsigned int line, const char* format, ...);
    bool isValid() const;
private:
    bool valid;
    int handle;

    
    char content[2][17], newContent[2][17];

    void setCursor(unsigned int x, unsigned int y);
    void print(char text);
    void update();
    
    PinClass& SDA;
    PinClass& SCL;

#ifdef NATIVE
    char buffer[32];
#endif
};

#endif // DISPLAY_HPP_INCLUDED