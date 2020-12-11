#include "listener_thread.h"
#include "socket_control.h"

#include <iostream>
#include <cstring>
#include <iomanip>
#include <vector>
#include <thread>

using namespace std;

//functions
int getBalance();
vector<string> getList();

//global variables
SocketControl *mainSocketControl;

int main(int argc, char *argv[])
{
  //connection initialization start
  mainSocketControl = new SocketControl;
  while (true)
  {
    char ip[100];
    int port;
    cout << "Please input server IP (v4): ";
    cin >> ip;
    cout << "Please input server port: ";
    cin >> port;
    if (mainSocketControl->bind(ip, port) == -1)
    {
      cout << "Connection failed, please check your ip and port input\n";
    }
    else
    {
      break;
    }
  }
  //connection initialization end

  //register start
  string username;
  cout << "\033c==============================\nRegister\nAvailable commands:\nnew - register new account\nnext - proceed to login\n==============================\n\n";
  while (true)
  {
    string command;
    cin >> command;
    if (command == "new")
    {
      int initialAmount;
      cout << "Please input user name to register: ";
      cin >> username;
      cout << "Please input initial amount: ";
      cin >> initialAmount;
      string sendString = "REGISTER#" + username + "#" + to_string(initialAmount) + "\n";
      string receiveString = mainSocketControl->sendCommand(sendString);
      if (receiveString == "210 FAIL\n")
      {
        cout << "User name already taken\n";
      }
      else
      {
        cout << "Register successful\n";
      }
    }
    else if (command == "next")
    {
      break;
    }
    else
    {
      cout << "Command unavailable\n";
      continue;
    }
  }
  //register end

  //login start
  cout << "\033c==============================\nLogin\n==============================\n\n";
  int clientPort;
  while (true)
  {
    cout << "Login user name: ";
    cin >> username;
    cout << "Client port to open: ";
    cin >> clientPort;
    if (clientPort >= 1024 && clientPort <= 65536)
    {
      string sendString = username + "#" + to_string(clientPort) + "\n";
      string receiveString = mainSocketControl->sendCommand(sendString);
      if (receiveString == "220 AUTH_FAIL\n")
      {
        cout << "Login failed, username not existent\n";
      }
      else if (receiveString == "This account has been logged in!\n")
      {
        cout << "Login failed, user has already logged in\n";
      }
      else
      {
        cout << "Login successful\n";
        break;
      }
    }
    else
    {
      cout << "Port number must be in range 1024~65536\n";
    }
  }
  ListenerThread *masterListenThread = new ListenerThread(clientPort, mainSocketControl);
  thread t1(&ListenerThread::startListen, masterListenThread);
  //login end

  //functionals start
  cout << "\033c==============================\nCommands\nAvailable commands:\nbalance - get current account balance\nlist - list current accounts\nsendtx - send transaction\nlogout - log out and exit\n==============================\n\n";
  while (true)
  {
    string command;
    cin >> command;
    if (command == "balance")
    {
      int balance = getBalance();
      cout << "Balance: " << balance << "\n";
    }
    else if (command == "list")
    {
      vector<string> clientList = getList();
      cout << "--------------------------------------\n";
      cout << "    Username       IP Address    Port \n";
      cout << "--------------------------------------\n";
      for (int i = 0; i < (clientList.size() + 1) / 3; i++)
      {
        cout << setw(16) << clientList[i * 3] << setw(16) << clientList[i * 3 + 1] << setw(6) << clientList[i * 3 + 2] << "\n";
      }
    }
    else if (command == "sendtx")
    {
      string recepientIp = "", recepientPort = "";
      string recepient;
      int amount;
      while (true)
      {
        cout << "Who do you want to send to? ";
        cin >> recepient;
        vector<string> userData = getList();
        for (int i = 0; i < (userData.size() + 1) / 3; i++)
        {
          if (userData[i * 3] == recepient)
          {
            recepientIp = userData[i * 3 + 1];
            recepientPort = userData[i * 3 + 2];
          }
        }
        if (recepientIp == "")
        {
          cout << "Non-existent user, please try again\n";
          continue;
        }
        cout << "Please specify amount to send: ";
        cin >> amount;
        if (amount > getBalance())
        {
          cout << "Insufficient balance, please try again\n";
          continue;
        }
        else
        {
          break;
        }
      }
      string sendString = username + "#" + to_string(amount) + "#" + recepient;
      SocketControl tmpSocketControl;
      char ipArr[100];
      strncpy(ipArr, recepientIp.c_str(), sizeof(recepientIp));
      int err = tmpSocketControl.bind(ipArr, stoi(recepientPort));
      string receiveString = tmpSocketControl.sendCommand(sendString);
      tmpSocketControl.terminate();
      cout << receiveString;
    }
    else if (command == "logout")
    {
      string sendString = "Exit";
      string receiveString = mainSocketControl->sendCommand(sendString);
      cout << receiveString;
      break;
    }
    else
    {
      cout << "Command unavailable\n";
      continue;
    }
  }
  //functionals end

  t1.join();
  cout << "Socket closed\n";
  mainSocketControl->terminate();
  return 0;
}

int getBalance()
{
  string sendString = "List";
  stringstream receiveString(mainSocketControl->sendCommand(sendString));
  string segment;
  vector<string> segments;
  segments.clear();
  while (getline(receiveString, segment, '\n'))
  {
    segments.push_back(segment);
  }
  return atoi(segments[0].c_str());
}

vector<string> getList()
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
