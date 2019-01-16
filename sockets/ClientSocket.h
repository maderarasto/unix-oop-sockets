#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

// Hlavicky pre pracu so socketmi
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>

// C++ systemove hlavicky
#include <string>
#include <cstring>
#include <stdexcept>

class ClientSocket
{
public:
    ClientSocket();
    ClientSocket(int socketFD);
    ~ClientSocket();

    bool isConnected() const;

    void connectTo(std::string hostname, int port);
    bool receive(std::string& message);
    void send(std::string& message);
    void disable();

public:
    static const int SmallBuffer;
    static const int BigBuffer;

private:
    int mSocketFD;
    bool mConnected;
    sockaddr_in mServerAddress;
};

#endif // CLIENTSOCKET_H