#include "lidar.hpp"    

#include "log.hpp"

#ifdef RASPBERRY_PI
    #include <wiringPiI2C.h>
    #include "wiringPi.h"
#endif

LiDARClass::LiDARClass(PinClass& SDA, PinClass& SCL, unsigned char address) : i2c(SDA, SCL, address)
{
    if ((address < 0x08) || (address > 0x77))
    {
        LOG_ERROR("LiDAR", "Invalid address specified.");
        return;
    }

    if (!i2c.isValid())
    {
        LOG_ERROR("LiDAR", "Invalid I2C.");
        return;
    }

    LOG_INFORMATION("LiDAR", "LiDAR initialized.");

    LOG_VERBOSE("LiDAR", "Firmware version: %s", this->getFirmwareVersion().c_str());
    LOG_VERBOSE("LiDAR", "Temperature: %d", this->getChipTemperature());

    if (this->getMode() == LiDARMode::CONTINUOUS)
    {
        LOG_VERBOSE("LiDAR", "Mode: Continuous");
    }
    else
    {
        LOG_VERBOSE("LiDAR", "Mode: Trigger");
    }

    LOG_VERBOSE("LiDAR", "Amplitude threshold: %d", this->getAmplitudeThreshold());
    LOG_VERBOSE("LiDAR", "Minimum distance: %d", this->getMinimumDistance());
    LOG_VERBOSE("LiDAR", "Maximum distance: %d", this->getMaximumDistance());

}

bool LiDARClass::isValid() const
{
    return i2c.isValid();
}

unsigned int LiDARClass::getAmplitude() const
{
    return i2c.readRegister(static_cast<int>(LiDARRegister::AMPLITUDE_HIGH));
}

unsigned int LiDARClass::getDistance() const
{
    return i2c.readRegister(static_cast<int>(LiDARRegister::DISTANCE_LOW));
}

float LiDARClass::getChipTemperature() const
{
    return ((float)i2c.readRegister(static_cast<int>(LiDARRegister::TEMPERATURE_LOW))) * 0.01;
}

std::string LiDARClass::getFirmwareVersion() const
{
    return std::to_string(i2c.readRegister(static_cast<int>(LiDARRegister::VERSION_MAJOR), true)) + "." +
        std::to_string(i2c.readRegister(static_cast<int>(LiDARRegister::VERSION_MINOR), true)) + "." +
        std::to_string(i2c.readRegister(static_cast<int>(LiDARRegister::VERSION_REVISION), true));
}

unsigned int LiDARClass::getError() const
{
    return i2c.readRegister(static_cast<int>(LiDARRegister::ERROR_LOW));
}

LiDARMode LiDARClass::getMode() const
{
    return static_cast<LiDARMode>(i2c.readRegister(static_cast<int>(LiDARRegister::MODE)));
}

void LiDARClass::setMode(LiDARMode mode)
{
    i2c.writeRegister(static_cast<int>(LiDARRegister::MODE), static_cast<int>(mode), true);
}

void LiDARClass::reboot()
{
    i2c.writeRegister(static_cast<int>(LiDARRegister::MODE), 0x02, true);
}

unsigned int LiDARClass::getAmplitudeThreshold() const
{
    return i2c.readRegister(static_cast<int>(LiDARRegister::AMPLITHUDE_THRESHOLD_LOW));
}

void LiDARClass::setAmplitudeThreshold(unsigned int threshold)
{
    i2c.writeRegister(static_cast<int>(LiDARRegister::AMPLITHUDE_THRESHOLD_LOW), threshold, true);
}

void LiDARClass::setMinimumDistance(unsigned int distance)
{
    i2c.writeRegister(static_cast<int>(LiDARRegister::MINIMUM_DISTANCE_LOW), distance, true);
}

void LiDARClass::setMaximumDistance(unsigned int distance)
{
    i2c.writeRegister(static_cast<int>(LiDARRegister::MAXIMUM_DISTANCE_LOW), distance, true);
}

unsigned int LiDARClass::getMinimumDistance() const
{
    return i2c.readRegister(static_cast<int>(LiDARRegister::MINIMUM_DISTANCE_LOW));
}

unsigned int LiDARClass::getMaximumDistance() const
{
    return i2c.readRegister(static_cast<int>(LiDARRegister::MAXIMUM_DISTANCE_LOW));
}

void LiDARClass::setOutputFrequency(unsigned int frequency)
{
    if (frequency > 250)
    {
        LOG_ERROR("LiDAR", "Frequency must be between 0 and 250.");
        return;
    }

    for (int i = 2; i < 500; i++)
    {
        if (frequency * i == 500)
        {
            break;
        }
        else if (i >= 499)
        {
            LOG_ERROR("LiDAR", "Frequency must be a multiple of 500.");
            return;
        }
    }

    i2c.writeRegister(static_cast<int>(LiDARRegister::FRAME_PER_SECOND_LOW), frequency, true);
}

unsigned int LiDARClass::getOutputFrequency() const
{
    return i2c.readRegister(static_cast<int>(LiDARRegister::FRAME_PER_SECOND_LOW));
}