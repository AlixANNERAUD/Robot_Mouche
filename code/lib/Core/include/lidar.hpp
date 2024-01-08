#pragma once

#include "pin.hpp"
#include "i2c.hpp"

#include <string>

enum class LiDARRegister : unsigned char
{
    DISTANCE_LOW = 0x00,
    DISTANCE_HIGH = 0x01,

    AMPLITUDE_LOW = 0x02,
    AMPLITUDE_HIGH = 0x03,

    TEMPERATURE_LOW = 0x04,
    TEMPERATURE_HIGH = 0x05,

    TICK_LOW = 0x06,
    TICK_HIGH = 0x07,

    ERROR_LOW = 0x08,
    ERROR_HIGH = 0x09,

    VERSION_REVISION = 0x0A,
    VERSION_MINOR = 0x0B,
    VERSION_MAJOR = 0x0C,

    SERIAL_NUMBER = 0x10,
    MODE = 0x23,
    TRIGGER_ONCE_SHOT = 0x24,
    ENABLE = 0x25,

    FRAME_PER_SECOND_LOW = 0x26,
    FRAME_PER_SECOND_HIGH = 0x27,

    LOW_POWER = 0x28,
    RESTORE_FACTORY_SETTINGS = 0x29,

    AMPLITHUDE_THRESHOLD_LOW = 0x2A,
    AMPLITHUDE_THRESHOLD_HIGH = 0x2B,

    DUMMY_DISTANCE_LOW = 0x2C,
    DUMMY_DISTANCE_HIGH = 0x2D,

    MINIMUM_DISTANCE_LOW = 0x2E,
    MINIMUM_DISTANCE_HIGH = 0x2F,

    MAXIMUM_DISTANCE_LOW = 0x30,
    MAXIMUM_DISTANCE_HIGH = 0x31,
};

enum class LiDARMode : unsigned char
{
    CONTINUOUS = 0x00,
    TRIGGER = 0x01,
};

class LiDARClass
{
public:
    LiDARClass() = delete;
    LiDARClass(PinClass &SDA, PinClass &SCL, unsigned char address = 0x10);
    LiDARClass(const LiDARClass &) = delete;
    LiDARClass &operator=(const LiDARClass &) = delete;

    bool isValid() const;

    std::string getFirmwareVersion() const;

    void reboot();

    void setMode(LiDARMode mode);
    void setAmplitudeThreshold(unsigned int threshold);
    void setMinimumDistance(unsigned int distance);
    void setMaximumDistance(unsigned int distance);
    void setOutputFrequency(unsigned int frequency);

    LiDARMode getMode() const;
    unsigned int getOutputFrequency() const;
    unsigned int getMinimumDistance() const;
    unsigned int getMaximumDistance() const;
    unsigned int getAmplitudeThreshold() const;
    unsigned int getAmplitude() const;
    unsigned int getDistance() const;
    unsigned int getError() const;
    float getChipTemperature() const;

private:
    I2CClass i2c;

    unsigned char address;
};