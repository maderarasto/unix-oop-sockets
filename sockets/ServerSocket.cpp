#include "ServerSocket.h"

const int ServerSocket::QueueSize = 4;

ServerSocket::ServerSocket(int port) :
    mListenerThread(nullptr)
{
    mPort = port;
    
    bzero((char*) &mAddressInfo, sizeof(mAddressInfo));
    mAddressInfo.sin_family = AF_INET;
    mAddressInfo.sin_addr.s_addr = INADDR_ANY;
    mAddressInfo.sin_port = port;

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
}

ServerSocket::~ServerSocket()
{
    if (mListenerThread->joinable())
        mListenerThread->join();
    
    delete mListenerThread;
}

int ServerSocket::getPendingFD()
{
    int socketFD = -1;

    mMutex.lock();
    
    if (mPendingFDs.size() > 0)
    {
        socketFD = mPendingFDs.front();
        mPendingFDs.erase(mPendingFDs.begin());
    }

    mMutex.unlock();

    return socketFD;
}

void ServerSocket::startListening()
{
    mListening = true;
    mListenerThread = new std::thread(&ServerSocket::handleListening, this);
}

void ServerSocket::stopListening()
{
    mListening = false;
    shutdown(mSocketFD, SHUT_RDWR);
    close(mSocketFD);
}

void ServerSocket::disable()
{
    printf("Closing a socket...\n");
    mListening = false;
    shutdown(mSocketFD, SHUT_RDWR);
    close(mSocketFD);
}

void ServerSocket::handleListening()
{
    listen(mSocketFD, QueueSize);
    printf("Starting to listen on port %d...\n", mPort);
    
    while (mListening)
    {
        sockaddr_in addressInfo;
        socklen_t addressLength = sizeof(addressInfo);

        int socketFD = accept(mSocketFD, (sockaddr*) &addressInfo, &addressLength);
        if (mListening && socketFD < 0)
            throw std::runtime_error("Accepting a new connection failed!");

        mMutex.lock();
        mPendingFDs.push_back(socketFD);
        mMutex.unlock();
    }
}