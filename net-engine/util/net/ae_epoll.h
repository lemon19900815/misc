#ifndef __AE_EPOLL_INC_H__
#define __AE_EPOLL_INC_H__

#include "aebase.h"

#ifndef _WIN32

#include <sys/epoll.h>

class aeEventEpoll : public aeEventBase
{
public:
  aeEventEpoll() { epollfd = -1; events = nullptr; }
  virtual ~aeEventEpoll() {}

  /*
  * create ae api event base with max event size
  */
  virtual aeEventLoop* aeApiCreate(int maxevents) override;

  /*
  * api event poll on timeout tv
  */
  virtual int aeApiPoll(aeEventLoop* eventLoop, struct timeval* tv) override;

  /*
  * add fd to event loop
  */
  virtual int aeApiAddEvent(aeEventLoop* eventLoop, int fd, int mask) override;

  /*
  * del fd mask on event loop
  */
  virtual int aeApiDelEvent(aeEventLoop* eventLoop, int fd, int delmask) override;

  /*
  * free ae event handler
  */
  virtual void aeApiFree(aeEventLoop* eventLoop) override;

  /*
  * ae api name
  */
  virtual const char* aeApiName() const override;

private:
  int epollfd;
  epoll_event* events;
};
#endif

#endif // __AE_EPOLL_INC_H__
