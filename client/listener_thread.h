// listener_thread.h
#ifndef LISTENER_THREAD_H
#define LISTENER_THREAD_H

class ListenerThread
{
public:
    ListenerThread(int);
    void startListen();

private:
    int listenerSocketDescriptor;
};

#endif