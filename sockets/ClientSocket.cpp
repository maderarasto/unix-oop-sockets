#include "ClientSocket.h"

const int ClientSocket::SmallBuffer = 10;
const int ClientSocket::BigBuffer = 100;

ClientSocket::ClientSocket()
{
    mConnected = false;
    bzero((char*) &mServerAddress, sizeof(mServerAddress));
}

ClientSocket::ClientSocket(int socketFD)
{
    mSocketFD = socketFD;
    mConnected = true;
    
}

ClientSocket::~ClientSocket()
{

}

bool ClientSocket::isConnected() const
{
    return mConnected;
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

    mConnected = true;
}

bool ClientSocket::receive(std::string& message)
{   
    char buffer[BigBuffer];
    
    bzero(buffer, BigBuffer);
    int receivedBytes = read(mSocketFD, buffer, ClientSocket::BigBuffer - 1);
    if (receivedBytes < 0)
    {
        throw std::runtime_error("Reading from the socket failed!");
    }

    message = buffer;
    return true;
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
    mConnected = false;
    shutdown(mSocketFD, SHUT_RDWR);
    close(mSocketFD);
}