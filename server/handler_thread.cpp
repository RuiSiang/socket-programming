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
    for (int i = 0; i < dataset->size(); i++)
    {
      if (dataset->at(i).username == username)
      {
        dataset->at(i).active = 0;
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
      for (int i = 0; i < dataset->size(); i++)
      {
        if (dataset->at(i).username == tmp.username)
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
        dataset->push_back(tmp);
        sendString = "100 OK\n";
      }
    }
    else if (segments[0] == "List")
    {
      cout << username << " requested list\n";
      for (int i = 0; i < dataset->size(); i++)
      {
        if (dataset->at(i).username == username)
        {
          sendString += to_string(dataset->at(i).balance) + '\n';
          break;
        }
      }
      sendString += to_string(dataset->size()) + '\n';
      for (int i = 0; i < dataset->size(); i++)
      {
        if (dataset->at(i).active)
        {
          sendString += dataset->at(i).username + '#' + dataset->at(i).ip + '#' + to_string(dataset->at(i).port) + '\n';
        }
      }
    }
    else
    {
      if (segments.size() == 2)
      {
        bool found = 0;
        for (int i = 0; i < dataset->size(); i++)
        {
          if (dataset->at(i).username == segments[0] && dataset->at(i).active == 0)
          {
            dataset->at(i).active = 1;
            dataset->at(i).ip = ip;
            dataset->at(i).port = stoi(segments[1]);
            for (int i = 0; i < dataset->size(); i++)
            {
              if (dataset->at(i).username == username)
              {
                sendString += to_string(dataset->at(i).balance) + '\n';
                break;
              }
            }
            sendString += to_string(dataset->size()) + '\n';
            for (int i = 0; i < dataset->size(); i++)
            {
              if (dataset->at(i).active)
              {
                sendString += dataset->at(i).username + '#' + dataset->at(i).ip + '#' + to_string(dataset->at(i).port) + '\n';
              }
            }
            cout << "User logged in: " << segments[0] << "\n";
            username = segments[0];
            found = 1;
            break;
          }
          else if (dataset->at(i).username == segments[0] && dataset->at(i).active == 1)
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
        cout << "Decrypting with prv\n";
        cout << "Decrypting with sender and receiver pub\n";
        cout << "Transaction content: " << segments[0] << " sent " << segments[1] << " to " << segments[2] << "\n";
        for (int i = 0; i < dataset->size(); i++)
        {
          if (dataset->at(i).username == segments[0])
          {
            dataset->at(i).balance -= stoi(segments[1]);
          }
          else if (dataset->at(i).username == segments[2])
          {
            dataset->at(i).balance -= stoi(segments[1]);
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
