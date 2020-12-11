#include "handler_thread.h"
#include "logger.h"

#include <iostream>
#include <cstring>
#include <vector>
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
      if (string(receiveData).length() < CHUNK_SIZE)
      {
        break;
      }
    }
    if (process(receiveString))
    {
      break;
    }
    memset(sendData, '\0', sizeof(sendData));
    sendString = "Payment received, retransmitting to server\n";
    strncpy(sendData, sendString.c_str(), sizeof(sendData));
    send(threadSocketDescriptor, sendData, sizeof(sendData), 0);
  }
  info("Thread " + to_string(threadSocketDescriptor) + " terminated\n");
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
    string segment;
    vector<string> segments;
    segments.clear();
    stringstream receiveStream(receiveString);
    while (getline(receiveStream, segment, '#'))
    {
      segments.push_back(segment);
    }
    string sender = segments[0];
    string amount = segments[1];
    info("Received incoming transaction: " + sender + " sent " + amount + ". Retransmitting to server\n");
    info(mainSocketControl->sendCommand(receiveString));
  }
  return 0;
}
