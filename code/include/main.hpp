#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include "pin.hpp"
#include "driver.hpp"
void display(PinClass& SDA, PinClass& SCL);
void server(DriverClass& driver);

#endif