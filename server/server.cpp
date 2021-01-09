#include "listener_thread.h"

#include <iostream>
#include <cstring>
#include <iomanip>
#include <vector>
#include <ssl_handler.h>

#ifdef _WIN32
#include <WinSock2.h>
#endif

#define CHUNK_SIZE 4096

using namespace std;

int main(int argc, char *argv[])
{
#ifdef WIN32
  WSADATA wsaData;
  WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
  unsigned int port;
  cout << "Please input server port: ";
  cin >> port;

  SslHandler *sslHandler = new SslHandler;
  ListenerThread masterThread(port, sslHandler);
  masterThread.startListen(10);

  return 0;
}