#include "ae_epoll.h"
#include "ae.h"
#include "networking.h"
#include "common.h"

#ifndef _WIN32
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h> // close

/*
* create ae api event base with max event size
*/
aeEventLoop* aeEventEpoll::aeApiCreate(int maxevents)
{
  aeEventLoop* ae = new aeEventLoop();
  ae->apiState = this;
  ae->maxfd = 0;
  ae->maxevents = maxevents;
  ae->stop = 0;
  ae->fired = new aeFiredEvent[maxevents];
  ae->events = new aeFileEvent[maxevents];

  epollfd = epoll_create(1024); /* 1024 is just a hint for the kernel */
  if (epollfd == -1) {
    aeApiFree(ae);
  } else {
    events = new epoll_event[maxevents];
    /*init file event mask flag*/
    for (int i = 0; i < maxevents; ++i) {
      ae->events[i].mask = AE_NONE;
    }
  }

  return ae;
}

/*
* api event poll on timeout tv
*/
int aeEventEpoll::aeApiPoll(aeEventLoop* eventLoop, struct timeval* tv)
{
  int retval = epoll_wait(epollfd,
    events, eventLoop->maxevents, tv ? (tv->tv_sec*1000 + tv->tv_usec/1000) : -1); // timeout -1 wait until events occur

  int numevents = 0;

  if (retval) {
    numevents = retval;

    int mask = AE_NONE;
    for (int i = 0; i < retval; ++i) {
      auto e = events + i;

      if (e->events & EPOLLIN) mask |= AE_READABLE;
      if (e->events & EPOLLOUT) mask |= AE_WRITABLE;
      if (e->events & EPOLLERR) mask |= AE_WRITABLE;
      if (e->events & EPOLLHUP) mask |= AE_WRITABLE;
      eventLoop->fired[i].fd = e->data.fd;
      eventLoop->fired[i].mask = mask;
    }
  }

  return numevents;
}

/*
* add fd to event loop
*/
int aeEventEpoll::aeApiAddEvent(aeEventLoop* eventLoop, int fd, int mask)
{
  int op = eventLoop->events[fd].mask == AE_NONE
    ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;

  epoll_event ev;
  ev.events = 0;

  mask |= eventLoop->events[fd].mask;
  if (mask & AE_READABLE) ev.events |= EPOLLIN;
  if (mask & AE_WRITABLE) ev.events |= EPOLLOUT;
  ev.data.u64 = 0; /* avoid valgrind warning */
  ev.data.fd = fd;

  if (epoll_ctl(epollfd, op, fd, &ev) == -1)
    return -1;

  return 0;
}

/*
* del fd mask on event loop
*/
int aeEventEpoll::aeApiDelEvent(aeEventLoop* eventLoop, int fd, int delmask)
{
  int mask = eventLoop->events[fd].mask & (~delmask);

  int op;
  epoll_event ev;
  ev.events = 0;
  ev.data.u64 = 0; /* avoid valgrind warning */
  ev.data.fd = fd;

  if (mask == AE_NONE) {
    op = EPOLL_CTL_DEL;
  } else {
    op = EPOLL_CTL_MOD;

    if (mask & AE_READABLE) ev.events |= EPOLLIN;
    if (mask & AE_WRITABLE) ev.events |= EPOLLOUT;
  }

  if (epoll_ctl(epollfd, op, fd, &ev) == -1)
    return -1;

  return 0;
}

/*
* free ae event handler
*/
void aeEventEpoll::aeApiFree(aeEventLoop* eventLoop)
{
  if (!eventLoop) return;

  SAFE_DELETE_ARRAY(eventLoop->events);
  SAFE_DELETE_ARRAY(eventLoop->fired);

  eventLoop->apiState = nullptr;
  SAFE_DELETE(eventLoop);

  if (epollfd != -1) {
    close(epollfd);
    epollfd = -1;
    SAFE_DELETE_ARRAY(events);
  }

  delete this;
}

/*
* ae api name
*/
const char* aeEventEpoll::aeApiName() const
{
  return "epoll";
}

#endif
