// handler_thread.h
#ifndef SOCKET_CONTROL_H
#define SOCKET_CONTROL_H

#include <string>
#include "ssl_handler.h"
class SocketControl
{
public:
  SocketControl(SslHandler *_sslHandler);
  int bind(char[], int);
  std::string sendCommand(std::string);
  void terminate();
  int socketDescriptor;

private:
  SslHandler *sslHandler;
};

#endif