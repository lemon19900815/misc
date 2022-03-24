#ifndef __AE_SELECT_INC_H__
#define __AE_SELECT_INC_H__

#include "aebase.h"

#ifdef _WIN32
#include <WinSock2.h>

class aeEventSelect : public aeEventBase
{
public:
  aeEventSelect() { }
  virtual ~aeEventSelect() { }

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
  fd_set rfds;
  fd_set wfds;

  /* We need to have a copy of the fd sets as it's not safe to reuse
  * FD sets after select(). */
  fd_set rfds_;
  fd_set wfds_;
};

#endif

#endif // __AE_SELECT_INC_H__
