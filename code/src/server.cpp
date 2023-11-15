#include "main.hpp"

#include "server.hpp"

#include <thread>

void server()
{
    ServerClass server = ServerClass();
    std::thread(&ServerClass::listen, server).detach();
}
