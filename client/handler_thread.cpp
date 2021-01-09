#include "handler_thread.h"
#include "logger.h"

#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include <thread>
#include <unistd.h>

#define CHUNK_SIZE 4096

#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#elif _WIN32
#include <WinSock2.h>
#endif

using namespace std;

void HandlerThread::handler()
{
  char sendData[CHUNK_SIZE], receiveData[CHUNK_SIZE];
  string sendString, receiveString;
  memset(sendData, '\0', sizeof(sendData));
  sendString = "Handler Assigned\n";
  strncpy(sendData, sendString.c_str(), sizeof(sendData));
  send(threadSocketDescriptor, sendData, sizeof(sendData), 0);
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
  process(receiveString);
  memset(sendData, '\0', sizeof(sendData));
  sendString = "Payment received, retransmitting to server\n";
  strncpy(sendData, sendString.c_str(), sizeof(sendData));
  send(threadSocketDescriptor, sendData, sizeof(sendData), 0);

  info("Thread " + to_string(threadSocketDescriptor) + " terminated\n");
  close(threadSocketDescriptor);

  delete this;
}

int HandlerThread::process(string receiveString)
{

  if (receiveString == "getpub")
  {
    info("Received getpub request");
    char sendData[CHUNK_SIZE];
    memset(sendData, '\0', sizeof(sendData));
    strncpy(sendData, (sslHandler->getPublicKey()).c_str(), sizeof(sendData));
    send(threadSocketDescriptor, sendData, sizeof(sendData), 0);
  }
  else
  {
    char sendData[CHUNK_SIZE], receiveData[CHUNK_SIZE];
    memset(sendData, '\0', sizeof(sendData));
    strncpy(sendData, string("getpub").c_str(), sizeof(sendData));
    SocketControl tmpSocketControl(sslHandler);
    string recepientPort = "";
    vector<string> userData = getList();
    for (int i = 0; i < (userData.size() + 1) / 3; i++)
    {
      if (userData[i * 3 + 1] == ip)
      {
        recepientPort = userData[i * 3 + 2];
      }
    }
    char ipArr[100];
    strncpy(ipArr, ip.c_str(), sizeof(recepientPort));
    int err = tmpSocketControl.bind(ipArr, stoi(recepientPort));
    send(tmpSocketControl.socketDescriptor, sendData, sizeof(sendData), 0);
    recv(tmpSocketControl.socketDescriptor, receiveData, sizeof(receiveData), 0);
    string pubkey = string(receiveData);

    receiveString = sslHandler->decryptMessage(receiveString);
    receiveString = sslHandler->prvDecryptMessage(receiveString, pubkey);
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
    info("Received incoming transaction: Signing and retransmitting to server\n");
    info(mainSocketControl->sendCommand(receiveString));
  }
  return 0;
}
vector<string> HandlerThread::getList()
{
  string sendString = "List";
  stringstream receiveStream(mainSocketControl->sendCommand(sendString));
  string tmp;
  vector<string> segments;
  segments.clear();
  while (getline(receiveStream, tmp, '\n'))
  {
    stringstream tmpss;
    string segment;
    tmpss.str(tmp);
    while (getline(tmpss, segment, '#'))
    {
      segments.push_back(segment);
    }
  }
  segments.erase(segments.begin(), segments.begin() + 2);
  return segments;
}
