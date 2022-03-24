#ifndef __AE_BASE_INC_H__
#define __AE_BASE_INC_H__

#include <cstdint>

#define AE_NONE 0
#define AE_READABLE 1
#define AE_WRITABLE 2

struct aeEventLoop;

class aeEventBase
{
public:
  aeEventBase() {}
  virtual~aeEventBase() {}

  /*
  * create ae api event base with max event size
  */
  virtual aeEventLoop* aeApiCreate(int maxevents) = 0;

  /*
  * api event poll on timeout tv
  */
  virtual int aeApiPoll(aeEventLoop* eventLoop, struct timeval* tv) = 0;

  /*
  * add fd to event loop
  */
  virtual int aeApiAddEvent(aeEventLoop* eventLoop, int fd, int mask) = 0;

  /*
  * del fd mask on event loop
  */
  virtual int aeApiDelEvent(aeEventLoop* eventLoop, int fd, int delmask) = 0;

  /*
  * free ae event handler
  */
  virtual void aeApiFree(aeEventLoop* eventLoop) = 0;

  /*
  * ae api name
  */
  virtual const char* aeApiName() const = 0;
};

#endif // __AE_BASE_INC_H__
