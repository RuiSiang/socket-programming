#include "listener_thread.h"

#include <iostream>
#include <cstring>
#include <iomanip>
#include <vector>

#define CHUNK_SIZE 100

using namespace std;

int main(int argc, char *argv[])
{
  unsigned int port;
  cout << "Please input server port: ";
  cin >> port;

  ListenerThread masterThread(port);
  masterThread.startListen(10);

  return 0;
}