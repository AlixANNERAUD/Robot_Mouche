#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include "pin.hpp"
#include "qtr.hpp"

void display(PinClass& SDA, PinClass& SCL);
void server(QTRClass *qtr1, QTRClass *qtr2);

#endif
