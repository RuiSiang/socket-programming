#include "handler_thread.h"

#include <iostream>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define CHUNK_SIZE 100

using namespace std;

void HandlerThread::handler()
{
  char sendData[CHUNK_SIZE], receiveData[CHUNK_SIZE];
  string sendString, receiveString;
  memset(sendData, '\0', sizeof(sendData));
  sendString = "Handler Assigned\n";
  strncpy(sendData, sendString.c_str(), sizeof(sendData));
  send(threadSocketDescriptor, sendData, sizeof(sendData), 0);
  while (true)
  {
    receiveString = "";
    while (true)
    {
      memset(receiveData, '\0', sizeof(receiveData));
      recv(threadSocketDescriptor, receiveData, sizeof(receiveData), 0);
      receiveString += string(receiveData);
      //cout << "**" + string(receiveData) + "**";
      if (string(receiveData).length() < CHUNK_SIZE)
      {
        break;
      }
    }
    if (process(receiveString))
    {
      break;
    }
  }
  cout << "Thread " << threadSocketDescriptor << " terminated\n";
  close(threadSocketDescriptor);

  delete this;
}

int HandlerThread::process(string receiveString)
{
  if (receiveString == "Exit")
  {
    return 1;
  }
  else
  {
    cout << receiveString;
  }
  return 0;
}
