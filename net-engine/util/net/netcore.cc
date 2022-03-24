#include "netcore.h"

#include "ae.h"
#include "aebase.h"
#include "networking.h"
#include "common.h"
#include "log/logger.h"

#include <cassert>

#ifdef _WIN32
#include <WinSock2.h> // timeval struct
#else
#include <sys/time.h>
#endif

NetCore::NetCore()
{
  ae_ = nullptr;
  thread_helper_ = nullptr;
}

NetCore::~NetCore()
{
  thread_helper_ = nullptr;

  if (ae_ && ae_->apiState) {
    ae_->apiState->aeApiFree(ae_);
  }

  network::anetReleaseNet();
}

bool NetCore::initialize(int maxeventsize, aeAccept onAccept, aeClose onClose)
{
  if (!ae_) {
    ae_ = aeEventLoop::create(maxeventsize);
    assert(ae_);

    ae_->acceptProc = onAccept;
    ae_->closeProc = onClose;

    if (-1 == network::anetInitializeNet()) {
      gLogger.logerr("initialize net failed[%s]", strerror(errno));
      return false;
    }

    gLogger.loginfo("network core initialized with[%s] module.", ae_->apiState->aeApiName());
  }

  return nullptr != ae_;
}

bool NetCore::startup(int port)
{
  assert(ae_);

  if (-1 == network::anetListenToPort(port, ae_->listenfds, &ae_->listenfdcount)) {
    gLogger.logerr("netcore start on[port:%d] falied.", port);
    return false;
  }

  auto acceptProc = std::bind(&acceptHandler, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

  // register fd file event
  for (int i = 0; i < ae_->listenfdcount; ++i) {
    if (ae_->aeCreateFileEvent(ae_->listenfds[i], AE_READABLE, acceptProc, nullptr) == -1) {
      gLogger.logerr("listen fd register file event failed");
      return false;
    }
  }

  thread_helper_ = std::make_shared<ThreadHelper>("NetThread", shared_from_this());
  thread_helper_->start();

  gLogger.loginfo("start network on[port:%d]", port);

  return true;
}

void NetCore::stop()
{
  gLogger.loginfo("stop network...");

  if (thread_helper_) {
    thread_helper_->stop();
    thread_helper_ = nullptr;
  }
}

// thread function
void NetCore::run(void* arg)
{
  NOTUSED(arg);

  timeval tv = { 0, 10000 };
  int fired = ae_->apiState->aeApiPoll(ae_, &tv);

  for (int i = 0; i < fired; ++i)
  {
    auto fe = &ae_->fired[i];
    auto e = &ae_->events[fe->fd];

    if (fe->mask & e->mask & AE_READABLE) {
      if (e->rfileProc) e->rfileProc(ae_, fe->fd, e->session, AE_READABLE);
    }
    if (fe->mask & e->mask & AE_WRITABLE) {
      if(e->wfileProc) e->wfileProc(ae_, fe->fd, e->session, AE_WRITABLE);
    }
  }
}
