#include "ServerSocket.h"

const int ServerSocket::ServerPort = 1259;
const int ServerSocket::QueueSize = 4;

ServerSocket::ServerSocket()
{
    bzero((char*) &mAddressInfo, sizeof(mAddressInfo));
    mAddressInfo.sin_family = AF_INET;
    mAddressInfo.sin_addr.s_addr = INADDR_ANY;
    mAddressInfo.sin_port = ServerSocket::ServerPort;

    // Vytvaranie socketu a ziskavanie deskriptora novo vytvoreneho socketu
    mSocketFD = socket(mAddressInfo.sin_family, SOCK_STREAM, 0);
    if (mSocketFD < 0)
    {
        throw std::runtime_error("Creating a server socket failed!");
    }

    if (bind(mSocketFD, (sockaddr*) &mAddressInfo, sizeof(mAddressInfo)) < 0)
    {
        throw std::runtime_error("Binding address to socket failed!");
    }

    updateBlockingFlags(false);    
}

ServerSocket::~ServerSocket()
{
}

int ServerSocket::onAccept()
{    
    fd_set acceptSet;
    int newSocketFD = -1;
    sockaddr_in clientAddressInfo;
    socklen_t clientLength = sizeof(clientAddressInfo);
    
    FD_ZERO(&acceptSet);
    FD_SET(mSocketFD, &acceptSet);
    listen(mSocketFD, ServerSocket::QueueSize);
    
    if (select(mSocketFD + 1, &acceptSet, NULL, NULL, NULL))
    {
        if (FD_ISSET(mSocketFD, &acceptSet))
        {
            newSocketFD = accept(mSocketFD, (sockaddr*) &clientAddressInfo, &clientLength);
            if (newSocketFD < 0)
            {
                throw std::runtime_error("Accepting a new connection failed!");
            }       
        }
    }

    return newSocketFD;
}

void ServerSocket::disable()
{
    printf("Closing a socket...\n");
    updateBlockingFlags(true);
    close(mSocketFD);
}

void ServerSocket::updateBlockingFlags(bool blocking)
{
    int flags = fcntl(mSocketFD, F_GETFL, 0);
    
    if (flags < 0)
    {
        throw std::runtime_error("Getting blocking flags failed!");
    }
    
    if (blocking)
        flags &= ~O_NONBLOCK;
    else
        flags |= O_NONBLOCK;

    if (fcntl(mSocketFD, F_SETFL, flags) < 0)
    {
        throw std::runtime_error("Setting blocking flags failed!");
    }
}