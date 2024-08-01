#include "../include/EchoServer.h"
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    EchoServer server(4, 10, "192.168.87.128", 8080);
    server.start();
    return 0;
}

