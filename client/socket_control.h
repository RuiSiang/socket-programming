// handler_thread.h
#ifndef SOCKET_CONTROL_H
#define SOCKET_CONTROL_H

#include<string>
class SocketControl
{
public:
  SocketControl();
  int bind(char[], int);
  std::string sendCommand(std::string);
  void terminate();

private:
  int socketDescriptor;
};

#endif