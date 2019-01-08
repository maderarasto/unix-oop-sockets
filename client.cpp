#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include "sockets/ClientSocket.h"

int main(int argc, char* argv[])
{
    std::string hostname = "127.0.0.1";
    int port = 1259;
    char buffer[100];
    std::string message;
    
    ClientSocket* clientSocket = new ClientSocket();
    clientSocket->setBlocking(false);
    clientSocket->connectTo(hostname, port);

    message = "Hello from socket";
    printf("Sending message: %s\n", message.c_str());
    //std::getline(std::cin, message);

    clientSocket->send(message);
    clientSocket->receive(message);

    printf("Message: %s\n", message.c_str());

    clientSocket->disable();
    
    return EXIT_SUCCESS;
}