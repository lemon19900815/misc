#ifndef __LOGGER_INC__
#define __LOGGER_INC__

#include <stdio.h>
#include <stdlib.h>

class Logger
{
public:
  Logger(const char* name);
  ~Logger();

  void loginfo(const char* format, ...);
  void logwarn(const char* format, ...);
  void logerr(const char* format, ...);

private:
  FILE* fp_;
};

extern Logger gLogger;

#endif // __LOGGER_INC__
