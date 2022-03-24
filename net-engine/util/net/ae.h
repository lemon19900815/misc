#ifndef __AE_INC_H__
#define __AE_INC_H__

#include <cstdint>
#include <memory>
#include <functional>

class aeEventBase;
struct aeEventLoop;

#include "session.h"

typedef std::function<void(aeEventLoop* eventLoop, int fd, Session::PTR session, int mask)> aeFileProc;

/* A fired event */
typedef struct aeFiredEvent {
  int fd;
  int mask;
} aeFiredEvent;

/* File event structure */
typedef struct aeFileEvent {
  int mask; /* one of AE_(READABLE|WRITABLE) */
  aeFileProc rfileProc;
  aeFileProc wfileProc;
  Session::PTR session; /*tcp session*/
} aeFileEvent;

typedef struct aeEventLoop {
  int maxfd; /* highest file descriptor currently registered */
  int maxevents; /* max number of file descriptors tracked */
  int stop;
  aeEventBase* apiState; /* This is used for polling API specific data */
  aeFiredEvent* fired; /* Fired events */
  aeFileEvent* events; /* Registered events */

  int listenfds[2];   // support ipv4 and ipv6 on same port, may the server machine only support ipv4 or ipv6
  int listenfdcount;

  aeAccept acceptProc; // notify netcore
  aeClose closeProc; // notify netcore

  /*
  * create platform aeEventLoop instance
  */
  static aeEventLoop* create(int maxevents);

  /*
  * release aeEventLoop instance
  */
  static void release(aeEventLoop* el);

  /*
  * create file event handle
  */
  int aeCreateFileEvent(int fd, int mask, aeFileProc proc, Session::PTR session);

  /*
  * clear event on fd
  */
  void clearEvent(int fd);

} aeEventLoop;

/*
* accept a tcp connect
*/
void acceptHandler(aeEventLoop* el, int fd, Session::PTR session, int mask);

/*
* read data from socket fd
*/
void readHandler(aeEventLoop* el, int fd, Session::PTR session, int mask);

/*
* write data on socket fd
*/
void writeHandle(aeEventLoop* el, int fd, Session::PTR session, int mask);

#endif // __AE_INC_H__
