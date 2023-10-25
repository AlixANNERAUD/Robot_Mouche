#include <stdio.h>
#include "log.hpp"
#include "pin.hpp"
#include "lcd.hpp"
#include "QTRSensors.hpp"

#include <thread>

#include "httplib.h"

void executeServer()
{
    httplib::Server svr;

    svr.Get("/hi", [](const httplib::Request &, httplib::Response &res)
            { res.set_content("Hello World!", "text/plain"); });

    svr.listen("0.0.0.0", 8080);
}

QTRSensors qtr;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

int main()
{
    if (!PinClass::initialize())
    {
        LOG_ERROR("Main", "Failed to initialize pin class.");
        return EXIT_FAILURE;
    }

    LOG_INFORMATION("Main", "Initialized pin class.");

    std::thread(executeServer).detach();

    PinClass Pin(2);

    Pin.setMode(PinMode::Output);

    LOG_INFORMATION("Main", "Ir sensors");
    qtr.setTypeRC();
    qtr.setSensorPins((const uint8_t[]){3, 4, 5, 6, 7, 8, 9, 10}, SensorCount);
    qtr.setEmitterPin(2);

    LOG_INFORMATION("Main", "Starting program.");

    while (true)
    {
        uint16_t position = qtr.readLineBlack(sensorValues);

        // print the sensor values as numbers from 0 to 1000, where 0 means maximum
        // reflectance and 1000 means minimum reflectance, followed by the line
        // position
        for (uint8_t i = 0; i < SensorCount; i++)
        {
            LOG_INFORMATION("Main", "%d", sensorValues[i]);
        }
        LOG_INFORMATION("Main", "%d", position);

    }
    

    return EXIT_SUCCESS;
}