#ifndef __NET_CORE_INC_H__
#define __NET_CORE_INC_H__

struct aeEventLoop;

#include "session.h"
#include "thread/runable.h"
#include "thread/threadhelper.h"

class NetCore : public IRunable, public std::enable_shared_from_this<NetCore>
{
public:
  NetCore();
  virtual ~NetCore();

  /*
  * initialize event module
  */
  bool initialize(int maxeventsize, aeAccept onAccept, aeClose onClose);

  /*
  * start network thread work
  */
  bool startup(int port);

  /*
  * stop network thread work
  */
  void stop();

  // thread function
  void run(void* arg) override;

private:
  aeEventLoop* ae_;
  ThreadHelper::PTR thread_helper_;
};

#endif // __NET_CORE_INC_H__
