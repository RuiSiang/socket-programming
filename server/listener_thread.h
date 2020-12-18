// listener_thread.h
#ifndef LISTENER_THREAD_H
#define LISTENER_THREAD_H

#include <vector>
#include <string>

struct Dataset {
    string username;
    int balance;
    string ip;
    int port;
    bool active;
};
class ListenerThread
{
public:
    ListenerThread(int);
    void startListen(int);

private:
    int listenerSocketDescriptor;
    vector<Dataset> dataset;
};

#endif