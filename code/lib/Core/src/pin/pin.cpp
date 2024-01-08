#include "pin.hpp"

#include "log.hpp"

#include <algorithm>

std::vector<PinClass*> PinClass::pins;

PinClass::PinClass(unsigned int pin) : pin(pin)
{
    // Check if pin is valid
//    if (convertToWiringPi(pin) == 0xFFFFFFFF)
//    {
//        LOG_ERROR("Pin", "Pin %i is not a valid pin.", pin);
//        this->wiringPiPin = 0xFFFFFFFF;
//        this->valid = false;
//        return;
//    }
    this->wiringPiPin = pin;
   // this->wiringPiPin = convertToWiringPi(pin);
    LOG_VERBOSE("Pin", "Pin %i is valid : %i", pin, this->wiringPiPin);
    // Check if pin is already in use
    for (auto pin : PinClass::pins)
    {
        if (*pin == *this)
        {
            LOG_ERROR("Pin", "Pin instance %i is already in use.", this->pin);
            this->valid = false;
            break;
        }
    }
    // Add pin to list of pins
    this->pins.push_back(this);
    
    this->valid = true;
}

PinClass::~PinClass()
{
    // Remove pin from list of pins
    pins.erase(std::remove(pins.begin(), pins.end(), this), pins.end());
}

bool PinClass::isValid() const
{
    return this->valid;
}

unsigned int PinClass::getPin() const
{
    return this->pin;
}

unsigned int PinClass::convertToWiringPi(unsigned int pin)
{
    switch (pin)
    {
    case 0:
        return 30;
    case 1:
        return 31;
    case 2:
        return 8;
    case 3:
        return 9;
    case 4:
        return 7;
    case 5:
        return 21;
    case 6:
        return 22;
    case 7:
        return 11;
    case 8:
        return 10;
    case 9:
        return 13;
    case 10:
        return 12;
    case 11:
        return 14;
    case 12:
        return 26;
    case 13:
        return 23;
    case 14:
        return 15;
    case 15:
        return 16;
    case 16:
        return 27;
    case 17:
        return 0;
    case 18:
        return 1;
    case 19:
        return 24;
    case 20:
        return 28;
    case 21:
        return 29;
    case 22:
        return 3;
    case 23:
        return 4;
    case 24:
        return 5;
    case 25:
        return 6;
    case 26:
        return 25;
    case 27:
        return 2;
    default:
        return 0xFFFFFFFF;
    }
}


bool PinClass::operator==(const PinClass& other) const
{
    return this->pin == other.pin;
}

bool PinClass::operator!=(const PinClass& other) const
{
    return !(this->pin != other.pin);
}