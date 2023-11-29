#include "qtr.hpp"

#include "log.hpp"

#include <chrono>
#include <thread>
#include <unistd.h>

QTRClass::QTRClass(PinClass &sensor1, PinClass &sensor2, PinClass &sensor3)
    : sensor1(sensor1),
      sensor2(sensor2),
      sensor3(sensor3),
      running(true)
{ 
    std::thread(&QTRClass::loop, this).detach();
}

QTRClass::~QTRClass()
{
    this->running = false;
}

void QTRClass::loop()
{
    for (int i = 0; i < 3; i++)
        start_times[i] = clock();

    chargeSensor(sensor1);
    chargeSensor(sensor2);
    chargeSensor(sensor3);

    while (this->running)
    {
        if (sensor1.readDigital() == DigitalState::Low)
        {
            chargeSensor(sensor1);
            this->start_times[0] = clock();
        }
        if (sensor2.readDigital() == DigitalState::Low)
        {
            chargeSensor(sensor2);
            this->start_times[1] = clock();
        }
        if (sensor3.readDigital() == DigitalState::Low)
        {
            chargeSensor(sensor3);
            this->start_times[2] = clock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

void QTRClass::chargeSensor(PinClass& sensor)
{
    sensor.setMode(PinMode::Output);
    sensor.writeDigital(DigitalState::High);
    usleep(10);
    sensor.setMode(PinMode::Input);
}

std::array<clock_t, 3> QTRClass::getTimesElapsed() const
{
    std::array<clock_t, 3> times;
    for (int i = 0; i < 3; i++)
        times[i] = clock() - this->start_times[i];
    return times;
}
