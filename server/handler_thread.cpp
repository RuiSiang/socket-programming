#include "handler_thread.h"

#include <iostream>
#include <sstream>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <sys/types.h>

#define CHUNK_SIZE 100

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
    for (int i = 0; i < dataset.size(); i++)
    {
      if (dataset[i].username == username)
      {
        dataset[i].active = 0;
      }
    }
    char sendData[CHUNK_SIZE];
    string sendString = "Bye";
    memset(sendData, '\0', sizeof(sendData));
    strncpy(sendData, sendString.c_str(), sizeof(sendData));
    send(threadSocketDescriptor, sendData, sizeof(sendData), 0);
    return 1;
  }
  else
  {
    //cout << receiveString;
    stringstream receiveStream(receiveString);
    string segment;
    vector<string> segments;
    segments.clear();
    while (getline(receiveStream, segment, '#'))
    {
      segments.push_back(segment);
    }
    string sendString = "";
    if (segments[0] == "REGISTER")
    {
      Dataset tmp;
      tmp.username = segments[1];
      tmp.balance = stoi(segments[2]);
      bool duplicate = 0;
      for (int i = 0; i < dataset.size(); i++)
      {
        if (dataset[i].username == tmp.username)
        {
          duplicate = 1;
          break;
        }
      }
      if (duplicate)
      {
        sendString = "210 FAIL\n";
      }
      else
      {
        cout << "User Registered: " << tmp.username << " with balance " << tmp.balance << "\n";
        dataset.push_back(tmp);
        sendString = "100 OK\n";
      }
    }
    else if (segments[0] == "List")
    {
      for (int i = 0; i < dataset.size(); i++)
      {
        if (dataset[i].username == username)
        {
          sendString += to_string(dataset[i].balance) + '\n';
          break;
        }
      }
      sendString += to_string(dataset.size()) + '\n';
      for (int i = 0; i < dataset.size(); i++)
      {
        if (dataset[i].active)
        {
          sendString += dataset[i].username + '#' + dataset[i].ip + '#' + to_string(dataset[i].port) + '\n';
        }
      }
    }
    else
    {
      if (segments.size() == 2)
      {
        bool found = 0;
        for (int i = 0; i < dataset.size(); i++)
        {
          if (dataset[i].username == segments[0] && dataset[i].active == 0)
          {
            dataset[i].active = 1;
            dataset[i].ip = ip;
            dataset[i].port = stoi(segments[1]);
            for (int i = 0; i < dataset.size(); i++)
            {
              if (dataset[i].username == username)
              {
                sendString += to_string(dataset[i].balance) + '\n';
                break;
              }
            }
            sendString += to_string(dataset.size()) + '\n';
            for (int i = 0; i < dataset.size(); i++)
            {
              if (dataset[i].active)
              {
                sendString += dataset[i].username + '#' + dataset[i].ip + '#' + to_string(dataset[i].port) + '\n';
              }
            }
            break;
          }
          else if (dataset[i].username == segments[0] && dataset[i].active == 1)
          {
            sendString = "This account has been logged in!\n";
            break;
          }
        }
        if (found == 0)
        {
          sendString = "220 AUTH_FAIL\n";
        }
      }
      else if (segments.size() == 3)
      {
        for (int i = 0; i < dataset.size(); i++)
        {
          if (dataset[i].username == segments[0])
          {
            dataset[i].balance -= stoi(segments[1]);
          }
          else if (dataset[i].username == segments[2])
          {
            dataset[i].balance -= stoi(segments[1]);
          }
        }
        sendString = "Transaction Successfully Submitted\n";
      }
    }
    char sendData[CHUNK_SIZE];
    int index = 0;
    while (true)
    {
      memset(sendData, '\0', sizeof(sendData));
      strncpy(sendData, sendString.substr(index * CHUNK_SIZE, CHUNK_SIZE).c_str(), sizeof(sendData));
      send(threadSocketDescriptor, sendData, sizeof(sendData), 0);
      if (sendData[CHUNK_SIZE - 1] == '\0')
      {
        break;
      }
      index++;
    }
  }
  return 0;
}
