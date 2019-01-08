#include "ClientSocket.h"

const int ClientSocket::SmallBuffer = 10;
const int ClientSocket::BigBuffer = 100;

ClientSocket::ClientSocket()
{
    mBlocking = true;
    bzero((char*) &mServerAddress, sizeof(mServerAddress));
}

ClientSocket::ClientSocket(int socketFD)
{
    mSocketFD = socketFD;
    mBlocking = true;
    
}

ClientSocket::~ClientSocket()
{

}

bool ClientSocket::getBlocking() const
{
    return mBlocking;
}

void ClientSocket::setBlocking(bool blocking)
{
    mBlocking = blocking;
    updateBlockingFlags();
}

void ClientSocket::connectTo(std::string hostname, int port)
{
    hostent* server = gethostbyname(hostname.c_str());
    if (server == nullptr)
    {
        throw std::runtime_error("Getting a host failed!");
    }

    mServerAddress.sin_family = AF_INET;
    bcopy((char*) server->h_addr, (char*) &mServerAddress.sin_addr.s_addr, server->h_length);
    mServerAddress.sin_port = port;

    mSocketFD = socket(mServerAddress.sin_family, SOCK_STREAM, 0);
    if (mSocketFD < 0)
    {
        throw std::runtime_error("Creating a socket failed!");
    }

    if (connect(mSocketFD, (sockaddr*) &mServerAddress, sizeof(mServerAddress)) < 0)
    {
        throw std::runtime_error("Connecting to a socket failed!");
    }
}

bool ClientSocket::receive(std::string& message)
{   
    fd_set readSet;
    int receivedBytes;
    char buffer[ClientSocket::BigBuffer];

    FD_ZERO(&readSet);
    FD_SET(mSocketFD, &readSet);

    if (!mBlocking && select(mSocketFD + 1, &readSet, NULL, NULL, NULL) > 0)
    {
        if (FD_ISSET(mSocketFD, &readSet))
        {
            bzero(buffer, ClientSocket::BigBuffer);
            receivedBytes = read(mSocketFD, buffer, ClientSocket::BigBuffer - 1);
            if (receivedBytes < 0)
            {
                throw std::runtime_error("Reading from the socket failed!");
            }

            // handle message
            message = buffer;
            return true;
        }
    } else if (mBlocking)
    {
        bzero(buffer, ClientSocket::BigBuffer);
        receivedBytes = read(mSocketFD, buffer, ClientSocket::BigBuffer - 1);
        if (receivedBytes < 0)
        {
            throw std::runtime_error("Reading from the socket failed!");
        }

        message = buffer;
        return true;
    }

    return false;
}

void ClientSocket::send(std::string& message)
{
    const char* msg = message.c_str();
    int receivedBytes = write(mSocketFD, msg, strlen(msg));
    if (receivedBytes < 0)
    {
        throw std::runtime_error("Writing on the socket failed!");
    }
}

void ClientSocket::disable()
{
    setBlocking(true);
    close(mSocketFD);
}

void ClientSocket::updateBlockingFlags()
{
    int flags = fcntl(mSocketFD, F_GETFL, 0);
    if (flags < 0)
    {
        throw std::runtime_error("Getting blocking flags failed!");
    }

    if (mBlocking)
        flags &= ~O_NONBLOCK;
    else 
        flags |= O_NONBLOCK;

    if (fcntl(mSocketFD, F_SETFL, flags) < 0)
    {
        throw std::runtime_error("Setting blocking flags failed!");
    }
}