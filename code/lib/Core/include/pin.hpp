#ifndef Abstraction_Hpp_Included
#define Abstraction_Hpp_Included

#include <vector>

enum class PinMode
{
    Input,
    Output,
    PwmOutput,
};

enum class DigitalState
{
    Low,
    High,
};

class PinClass {
    public:
        PinClass(unsigned int pin);
        ~PinClass();

        void setMode(PinMode mode);
        void writeDigital(DigitalState value);
        DigitalState readDigital() const;
        void writeAnalog(unsigned int value);
        int readAnalog() const;

        bool isValid() const;
        unsigned int getPin() const;

        bool operator==(const PinClass& other) const;
        bool operator!=(const PinClass& other) const;

        static bool initialize();
        static unsigned int convertToWiringPi(unsigned int pin);

    private:        
        unsigned int pin;
        bool valid;

#ifdef NATIVE
        PinMode mode;

        union {
            int analog_value;
            DigitalState digital_value;
        };
#endif

        static std::vector<PinClass*> pins;
};

#endif