// handler_thread.h
#ifndef HANDLER_THREAD_H
#define HANDLER_THREAD_H

#include <string>
#include "socket_control.h"
#include "ssl_handler.h"

class HandlerThread
{
public:
  HandlerThread(int descriptor, SocketControl *_mainsocketControl, SslHandler *_sslHandler)
  {
    threadSocketDescriptor = descriptor;
    mainSocketControl = _mainsocketControl;
    sslHandler = _sslHandler;
  }
  void handler();

private:
  int threadSocketDescriptor;
  SocketControl *mainSocketControl;
  SslHandler *sslHandler;
  int process(std::string);
};

#endif