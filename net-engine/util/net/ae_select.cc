#include "ae_select.h"
#include "ae.h"
#include "networking.h"
#include "common.h"

#ifdef _WIN32

/*
* create ae api event base with max event size
*/
aeEventLoop* aeEventSelect::aeApiCreate(int maxevents)
{
  aeEventLoop* ae = new aeEventLoop();
  ae->apiState = this;
  ae->maxfd = 0;
  ae->maxevents = maxevents;
  ae->stop = 0;
  ae->fired = new aeFiredEvent[maxevents];
  ae->events = new aeFileEvent[maxevents];
  ae->acceptProc = nullptr;
  ae->closeProc = nullptr;

  /*init file event mask flag*/
  for (int i = 0; i < maxevents; ++i) {
    ae->events[i].mask = AE_NONE;
  }

  FD_ZERO(&rfds);
  FD_ZERO(&wfds);

  return ae;
}

/*
* api event poll on timeout tv
*/
int aeEventSelect::aeApiPoll(aeEventLoop* eventLoop, struct timeval* tv)
{
  int numevents = 0;

  memcpy(&rfds_, &rfds, sizeof(fd_set));
  memcpy(&wfds_, &wfds, sizeof(fd_set));

  int retval = select(eventLoop->maxfd + 1, &rfds_, &wfds_, nullptr, tv);

  if (retval > 0) {
    for (int fd = 0; fd <= eventLoop->maxfd; ++fd) {
      int mask = 0;
      aeFileEvent* fe = eventLoop->events + fd;

      if (fe->mask == AE_NONE) continue;
      if (fe->mask && FD_ISSET(fd, &rfds_)) mask |= AE_READABLE;
      if (fe->mask && FD_ISSET(fd, &wfds_)) mask |= AE_WRITABLE;

      eventLoop->fired[numevents].fd = fd;
      eventLoop->fired[numevents].mask = mask;

      ++numevents;
    }
  }

  return numevents;
}

/*
* add fd to event loop
*/
int aeEventSelect::aeApiAddEvent(aeEventLoop* eventLoop, int fd, int mask)
{
  NOTUSED(eventLoop);

  if (fd >= FD_SETSIZE) return -1;

  if (mask & AE_WRITABLE) FD_SET(fd, &wfds);
  if (mask & AE_READABLE) FD_SET(fd, &rfds);

  return 0;
}

/*
* del fd mask on event loop
*/
int aeEventSelect::aeApiDelEvent(aeEventLoop* eventLoop, int fd, int delmask)
{
  NOTUSED(eventLoop);

  if (fd >= FD_SETSIZE) return -1;

  if (delmask & AE_WRITABLE) FD_CLR(fd, &wfds);
  if (delmask & AE_READABLE) FD_CLR(fd, &rfds);

  return 0;
}

/*
* free ae event handler
*/
void aeEventSelect::aeApiFree(aeEventLoop* eventLoop)
{
  if (!eventLoop) return;

  SAFE_DELETE_ARRAY(eventLoop->events);
  SAFE_DELETE_ARRAY(eventLoop->fired);

  eventLoop->apiState = nullptr;
  SAFE_DELETE(eventLoop);

  delete this;
}

/*
* ae api name
*/
const char* aeEventSelect::aeApiName() const
{
  return "select";
}

#endif

