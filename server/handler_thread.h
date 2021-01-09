// handler_thread.h
#ifndef HANDLER_THREAD_H
#define HANDLER_THREAD_H

#include <string>
#include <vector>
#include "listener_thread.h"
#include "ssl_handler.h"

class HandlerThread
{
public:
  HandlerThread(int descriptor, std::vector<Dataset> *_dataset, std::string _ip, SslHandler *_sslHandler)
  {
    threadSocketDescriptor = descriptor;
    dataset = _dataset;
    ip = _ip;
    sslHandler = _sslHandler;
  }
  void handler();

private:
  int threadSocketDescriptor;
  int process(std::string);
  std::vector<Dataset> *dataset;
  SslHandler *sslHandler;
  std::string username;
  std::string ip;
};

#endif
