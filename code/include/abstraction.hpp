#ifndef Abstraction_Hpp_Included
#define Abstraction_Hpp_Included

#ifdef RASPBERRY_PI_4
    #include <wiringPi.h>
#endif

class VirtualPinClass{
    public:
        VirtualPinClass() = default;

        int mode = 0;
        int value = 0;
};

class GpioClass {
    public:
        GpioClass() = default;
        GpioClass(const GpioClass&) = delete;

        bool initialize();

        void pinMode(int pin, int mode);

        void digitalWrite(int pin, int value);
        int digitalRead(int pin);

        void analogWrite(int pin, int value);
        int analogRead(int pin);

    private:

#ifdef NATIVE
    bool initialized;
    VirtualPinClass pins[40];
#endif
};

extern GpioClass Gpio;

#endif
