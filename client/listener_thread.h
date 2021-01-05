// listener_thread.h
#ifndef LISTENER_THREAD_H
#define LISTENER_THREAD_H
#include "socket_control.h"
#include "handler_thread.h"

class ListenerThread
{
public:
    ListenerThread(int, SocketControl *, SslHandler *);
    void startListen();

private:
    int listenerSocketDescriptor;
    SocketControl *mainSocketControl;
    SslHandler *sslHandler;
};

#endif