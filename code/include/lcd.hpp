#ifndef DISPLAY_HPP_INCLUDED
#define DISPLAY_HPP_INCLUDED

#include "pin.hpp"

class LCDClass
{
public:
    LCDClass() = delete;
    LCDClass(PinClass& SDA, PinClass& SCL);

    void clear();
    void setCursor(unsigned int x, unsigned int y);
    void printFormatted(const char* format, ...);

    bool isValid() const;
private:
    bool valid;
    int handle;
    
    PinClass& SDA;
    PinClass& SCL;

#ifdef NATIVE
    unsigned int x;
    unsigned int y;

    char buffer[32];
#endif
};

#endif // DISPLAY_HPP_INCLUDED