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
#include <vector>
#include <thread>
#include <mutex>

class ServerSocket
{
public:
    ServerSocket(int port);
    ~ServerSocket();

    int getPendingFD();

    void startListening();
    void stopListening();
    void disable();

private: 
    void handleListening();

public:
    static const int QueueSize;

private:
    int mPort;
    int mSocketFD;
    sockaddr_in mAddressInfo;
    std::vector<int> mPendingFDs;

    bool mListening;
    std::thread* mListenerThread;
    std::mutex mMutex;

};

#endif // SERVERSOCKET_H