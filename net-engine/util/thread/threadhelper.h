#ifndef __THREAD_HELPER_INC_H__
#define __THREAD_HELPER_INC_H__

#include "runable.h"

#include <thread>
#include <cstring>
#include <atomic>
#include <memory>

class ThreadHelper final
{
public:
  typedef std::shared_ptr<ThreadHelper> PTR;
  typedef std::weak_ptr<ThreadHelper> WPTR;

public:
  ThreadHelper(const char* name, IRunable::PTR runable) {
    name_ = name;
    runable_ = runable;
    stoped_.store(true);
  }

  ~ThreadHelper() {
    runable_ = nullptr;
    stoped_.store(true);
  }

  /*
  * start thread work
  */
  bool start() {
    if (stoped_.load()) {
      thread_ = std::make_shared<std::thread>(std::bind(&ThreadHelper::run, this));
      stoped_.store(false);
    }
    return true;
  }

  /*
  * stop thread work
  */
  void stop() {
    stoped_.store(true);
  }

  /*
  * yield to other thread to work
  */
  static void yield() {
    std::this_thread::yield();
  }

  /*
  * current work thread sleep ms
  */
  static void sleep(unsigned long long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
  }

private:
  void run() {
    while (!stoped_.load()) {
      runable_->run(nullptr);
      ThreadHelper::sleep(10);
    }
  }

private:
  std::shared_ptr<std::thread> thread_;

  std::string name_;
  IRunable::PTR runable_;
  std::atomic<bool> stoped_;
};

#endif // __THREAD_HELPER_INC_H__
