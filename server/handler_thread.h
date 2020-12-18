// handler_thread.h
#ifndef HANDLER_THREAD_H
#define HANDLER_THREAD_H

#include <string>
#include <vector>
#include "listener_thread.h"

class HandlerThread
{
public:
  HandlerThread(int descriptor, std::vector<Dataset> *_dataset)
  {
    threadSocketDescriptor = descriptor;
    dataset = *_dataset;
  }
  void handler();

private:
  int threadSocketDescriptor;
  int process(std::string);
  std::vector<Dataset> dataset;
};

#endif
