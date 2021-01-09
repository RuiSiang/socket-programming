// handler_thread.h
#ifndef HANDLER_THREAD_H
#define HANDLER_THREAD_H

#include <string>
#include <vector>
#include "socket_control.h"
#include "ssl_handler.h"

class HandlerThread
{
public:
  HandlerThread(int descriptor, SocketControl *_mainsocketControl, std::string _ip, SslHandler *_sslHandler)
  {
    threadSocketDescriptor = descriptor;
    mainSocketControl = _mainsocketControl;
    ip = _ip;
    sslHandler = _sslHandler;
  }
  void handler();

private:
  int threadSocketDescriptor;
  SocketControl *mainSocketControl;
  SslHandler *sslHandler;
  std::string ip;
  int process(std::string);
  std::vector<std::string> getList();
};

#endif