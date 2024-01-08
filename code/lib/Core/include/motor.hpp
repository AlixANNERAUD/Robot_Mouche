#pragma once

#include "pin.hpp"

enum class MotorDirection
{
    Forward,
    Backward
};

class MotorClass
{
public:
    MotorClass() = delete;
    MotorClass(PinClass& Enabled, PinClass& A1, PinClass& A2);

    void setSpeed(unsigned int speed);
    void setDirection(MotorDirection forward);
    void set(float relativeSpeed);
    void stop();

    bool isValid() const;

private:
    bool valid;

    PinClass& A1, A2, Enabled;

#ifdef NATIVE
    unsigned int speed;
    MotorDirection direction;
#endif

};