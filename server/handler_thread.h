// handler_thread.h
#ifndef HANDLER_THREAD_H
#define HANDLER_THREAD_H

#include <string>

class HandlerThread
{
public:
  HandlerThread(int descriptor)
  {
    threadSocketDescriptor = descriptor;
  }
  void handler();

private:
  int threadSocketDescriptor;
  int process(std::string);
};

#endif