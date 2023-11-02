#include "i2c.hpp"

#include "log.hpp"

#ifdef RASPBERRY_PI
#include "wiringPi.h"
#include <wiringPiI2C.h>
#endif

I2CClass::I2CClass(PinClass &SDA, PinClass &SCL, char address) : handle(-1), SDA(SDA), SCL(SCL)
{
    if (!SDA.isValid() || !SCL.isValid())
    {
        LOG_ERROR("I2C", "Invalid pin(s) specified.");
        return;
    }

    this->SDA = SDA;
    this->SCL = SCL;

#ifdef RASPBERRY_PI
    this->handle = wiringPiI2CSetup(address);
#else
    this->Handle = 0;
#endif

    if (!this->isValid())
    {
        LOG_ERROR("I2C", "Failed to initialize I2C.");
        return;
    }

    LOG_INFORMATION("I2C", "I2C initialized.");
}

bool I2CClass::isValid() const
{
    return this->handle != -1;
}

int I2CClass::read() const
{
#ifdef RASPBERRY_PI
    return wiringPiI2CRead(this->handle);
#else
    return 0;
#endif
}

int I2CClass::write(int data)
{
#ifdef RASPBERRY_PI
    return wiringPiI2CWrite(this->handle, data);
#else
    return 0;
#endif
}

int I2CClass::writeRegister(int registerAddress, int data, bool byte)
{
#ifdef RASPBERRY_PI
    if (byte)
        return wiringPiI2CWriteReg8(this->handle, registerAddress, data);
    else
        return wiringPiI2CWriteReg16(this->handle, registerAddress, data);
#else
    return 0;
#endif
}

int I2CClass::readRegister(int registerAddress, bool byte) const
{
#ifdef RASPBERRY_PI
    if (byte)
        return wiringPiI2CReadReg8(this->handle, registerAddress);
    else
        return wiringPiI2CReadReg16(this->handle, registerAddress);
#else
    return 0;
#endif
}
