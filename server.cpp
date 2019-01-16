#include <cstdio>
#include <cstdlib>
#include <string>

#include "sockets/ServerSocket.h"
#include "sockets/ClientSocket.h"

int main(int argc, char* argv[])
{
    bool running = true;
    int clientCount = 0;
    int maxClients = 2;
    
    printf("Creating socket for server...\n");
    ServerSocket* serverSocket = new ServerSocket(1259);
    serverSocket->startListening();
    ClientSocket* client = nullptr; 

    while (running)
    {
        std::string message;
        
        if (clientCount < maxClients)
        {
            int newSocketFD = serverSocket->getPendingFD();
            if (newSocketFD >= 0)
            {
                printf("New connection established...\n");
                client = new ClientSocket(newSocketFD);
                clientCount++;
            }

            if (clientCount == maxClients)
            {
                printf("Server socket is stopping to listen...\n");
                serverSocket->stopListening();
            }
        }

        if (client != nullptr)
        {
            printf("Checking if the server received message from the client...\n");
            if (client->receive(message))
            {
                printf("Here is the message: %s\n", message.c_str());
            }

            message = "I got your message";
            
            printf("Sending acknowledgement about request...\n");
            client->send(message);

            running = false;
        }
    }

    // cleaning
    printf("Closing sockets...\n");
    serverSocket->disable();
    client->disable();

    delete client;
    delete serverSocket;
    
    return EXIT_SUCCESS;
}