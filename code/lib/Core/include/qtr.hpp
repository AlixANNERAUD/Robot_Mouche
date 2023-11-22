#ifndef QTR_HPP_INCLUDED
#define QTR_HPP_INCLUDED

#include "pin.hpp"

#include <atomic>
#include <chrono>
#include <array>

class QTRClass
{
public:
    QTRClass(PinClass& sensor1, PinClass& sensor2, PinClass& sensor3);
    ~QTRClass();
    void loop();
    std::array<clock_t, 3> getTimesElapsed() const;

private:
    void chargeSensor(PinClass& sensor);
    
    std::atomic_bool running;
    std::atomic<clock_t> start_times[3];
    PinClass& sensor1, sensor2, sensor3;
};

#endif