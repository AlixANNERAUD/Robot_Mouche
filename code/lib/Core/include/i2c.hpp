#pragma once

#include "pin.hpp"

class I2CClass
{
public:
    I2CClass() = delete;
    I2CClass(PinClass &SDA, PinClass &SCL, char address);
    I2CClass(const I2CClass &) = delete;
    I2CClass &operator=(const I2CClass &) = delete;

    bool isValid() const;

    int read() const;
    int write(int data);
    int readRegister(int registerAddress, bool byte = false) const;
    int writeRegister(int registerAddress, int data, bool byte = false);

private:
    int handle;

    PinClass &SDA;
    PinClass &SCL;
};