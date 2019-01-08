#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

// Hlavicky pre pracu so socketmi
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>

// C++ systemove hlavicky
#include <stdexcept>
#include <cstring>
#include <cstdio>

class ServerSocket
{
public:
    ServerSocket();
    ~ServerSocket();

    int onAccept();
    void disable();

private: 
    void updateBlockingFlags(bool blocking);

public:
    static const int ServerPort;
    static const int QueueSize;

private:
    int mSocketFD;
    sockaddr_in mAddressInfo;
};

#endif // SERVERSOCKET_H