#include "ae.h"

#include "aebase.h"
#include "ae_epoll.h"
#include "ae_select.h"
#include "networking.h"

#include "timehelp.h"
#include "log/logger.h"

/*
* create platform aeEventLoop instance
*/
aeEventLoop* aeEventLoop::create(int maxevents)
{
#ifdef _WIN32
  auto apiState = new aeEventSelect();
#else
  auto apiState = new aeEventEpoll();
#endif

  return apiState->aeApiCreate(maxevents);
}

/*
* release aeEventLoop instance
*/
void aeEventLoop::release(aeEventLoop* el)
{
  auto apiState = el->apiState;
  apiState->aeApiFree(el);
}

int aeEventLoop::aeCreateFileEvent(int fd, int mask, aeFileProc proc, Session::PTR session)
{
  aeFileEvent * fe;

  if (fd >= maxevents) {
    errno = ERANGE;
    return -1;
  }

  fe = &events[fd];

  if (apiState->aeApiAddEvent(this, fd, mask) == -1)
    return -1;

  fe->mask |= mask;
  if (mask & AE_READABLE) fe->rfileProc = proc;
  if (mask & AE_WRITABLE) fe->wfileProc = proc;
  fe->session = session;
  if (fd > maxfd) maxfd = fd;

  return 0;
}

/*
* clear event on fd
*/
void aeEventLoop::clearEvent(int fd)
{
  auto& tevent = events[fd];

  tevent.mask = AE_NONE;
  tevent.session = nullptr;  // release shared_ptr, it will call destructor immediately
  tevent.rfileProc = nullptr;
  tevent.wfileProc = nullptr;
}

/*
* accept a tcp connect
*/
void acceptHandler(aeEventLoop* el, int fd, Session::PTR session, int mask)
{
  NOTUSED(session);
  NOTUSED(mask);

  char ip[IP_ADDRSTRLEN];
  int port;

  int cfd = network::anetTcpAccept(nullptr, fd, ip, sizeof(ip), &port);
  if (cfd != -1) {
    auto psession = std::make_shared<Session>(cfd, ip, port);
    auto rproc = std::bind(&readHandler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    auto wproc = std::bind(&writeHandle, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

    if (el->aeCreateFileEvent(cfd, AE_READABLE, rproc, psession) == -1) {
      network::anetClose(cfd);
    }
    else if (el->aeCreateFileEvent(cfd, AE_WRITABLE, wproc, psession) == -1) {
      el->clearEvent(cfd); // add read event success, but add write event failed, so need clear it
      network::anetClose(cfd);
    }
    else {
      gLogger.loginfo("accept: %s", psession->description().c_str());

      // notify netcore
      if (el->acceptProc) el->acceptProc(psession);
    }
  }
}

/*
* read data from socket fd
*/
void readHandler(aeEventLoop* el, int fd, Session::PTR session, int mask)
{
  NOTUSED(mask);

  // read error
  if (session->onRead() == -1) {
    gLogger.loginfo("close: %s", session->description().c_str());

    // notify netcore
    if (el->closeProc) el->closeProc(session);

    // session destructor call onClose
    //session->onClose();

    // del event on fd[there we close it]
    el->apiState->aeApiDelEvent(el, fd, AE_READABLE & AE_WRITABLE);

    // clear event
    el->clearEvent(fd);
  }
}

/*
* write data on socket fd
*/
void writeHandle(aeEventLoop* el, int fd, Session::PTR session, int mask)
{
  NOTUSED(mask);

  // read error
  if (session->onWrite() == -1) {
    gLogger.loginfo("close: %s", session->description().c_str());

    // notify netcore
    if (el->closeProc) el->closeProc(session);

    // session destructor call onClose
    //session->onClose();

    // del event on fd[there we close it]
    el->apiState->aeApiDelEvent(el, fd, AE_READABLE & AE_WRITABLE);

    // clear event
    el->clearEvent(fd);
  }
}
