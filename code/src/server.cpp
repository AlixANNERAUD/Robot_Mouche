#include "main.hpp"

#include "server.hpp"
#include "qtr.hpp"

#include <thread>

void server(QTRClass *qtr1, QTRClass *qtr2)
{
    ServerClass server = ServerClass(qtr1, qtr2);
    std::thread(&ServerClass::listen, server).detach();
}
