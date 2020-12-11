// listener_thread.h
#ifndef LISTENER_THREAD_H
#define LISTENER_THREAD_H

class ListenerThread
{
public:
    ListenerThread(int);
    void startListen(int);

private:
    int listenerSocketDescriptor;
};

#endif