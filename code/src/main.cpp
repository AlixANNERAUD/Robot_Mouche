#include <stdio.h>
#include "log.hpp"
#include "pin.hpp"
#include "lcd.hpp"

#include <thread>

#include "httplib.h"

void executeServer()
{
    httplib::Server svr;

    svr.Get("/hi", [](const httplib::Request &, httplib::Response &res)
            { res.set_content("Hello World!", "text/plain"); });

    svr.listen("0.0.0.0", 8080);
}

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

    LOG_INFORMATION("Main", "Starting program.");

    while (true)
    {
        
    }
    

    return EXIT_SUCCESS;
}