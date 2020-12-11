#include "listener_thread.h"
#include "handler_thread.h"
#include "logger.h"

#include <iostream>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define LISTENER_NUM 10

using namespace std;

ListenerThread::ListenerThread(int port)
{
  //socket initialization start
  listenerSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (listenerSocketDescriptor == -1)
  {
    error("Listener Socket Creation Failed\n");
    exit(-1);
  }
  else
  {
    info("Listener Established\n");
  }
  //socket initialization end

  //connection initialization start
  struct sockaddr_in listenerInfo;
  memset(&listenerInfo, 0, sizeof(listenerInfo));
  listenerInfo.sin_family = PF_INET;
  listenerInfo.sin_addr.s_addr = INADDR_ANY;
  listenerInfo.sin_port = htons(port);
  int err = bind(listenerSocketDescriptor, (struct sockaddr *)&listenerInfo, sizeof(listenerInfo));
  if (err == -1)
  {
    error("Bind port failed, please try again\n");
    exit(-1);
  }
  else
  {
    info("Binded on port " + port + "\n");
  }
  //connection initialization end
}
void ListenerThread::startListen()
{
  listen(listenerSocketDescriptor, LISTENER_NUM);
  while (true)
  {
    struct sockaddr_in incomingClientInfo;
    unsigned int infoSize = sizeof(incomingClientInfo);
    int incomingClientSocketDescriptor = accept(listenerSocketDescriptor, (struct sockaddr *)&incomingClientInfo, &infoSize);
    if (incomingClientSocketDescriptor <= 0)
    {
      break;
    }
    info("Incoming request assigned with descriptor " + incomingClientSocketDescriptor + " ");
    info("(originated from ip: " + inet_ntoa(incomingClientInfo.sin_addr) + ", port: "+ntohs(incomingClientInfo.sin_port)+")\n");

    HandlerThread *newThread = new HandlerThread(incomingClientSocketDescriptor);
    thread sth(&HandlerThread::handler, newThread);
    sth.detach();
  }

  close(listenerSocketDescriptor);
}
