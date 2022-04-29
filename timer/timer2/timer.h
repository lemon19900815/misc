#include <iostream>
#include <cstdint>
#include <chrono>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <list>
#include <queue>
#include <memory>
#include <sstream>

int64_t nowMsTime() {
  using namespace std::chrono;
  auto ms_dura = duration_cast<milliseconds>(
    system_clock::now().time_since_epoch());
  return ms_dura.count();
}

int32_t nowSecTime() {
  return nowMsTime() / 1000;
}

class Timer {
public:
  using Ptr = std::shared_ptr<Timer>;
  using Wptr = std::weak_ptr<Timer>;

public:
  Timer(int32_t ms_interval, std::function<void()>&& cb,
      int32_t delay = 0, int32_t repeat = 1) {

    static int32_t kTimerId = 1;

    time_out_ = -1;
    interval_ = ms_interval;
    timer_id_ = kTimerId++;
    repeat_ = repeat;
    cb_ = cb;
    time_out_ = nowMsTime() + delay + ms_interval;
  }

  void Start() {
    time_out_ += interval_;
  }

  int32_t GetLeftTime() {
    return time_out_ - nowMsTime();
  }

  void Execute() {
    if (!cb_) return;

    Trigger();

    auto now = nowMsTime();
    while (now - time_out_ >= interval_ && repeat_ > 0) {
      Trigger();
      time_out_ += interval_;
      std::cout << toString() << std::endl;
    }
  }

  void Cancel() {
    cb_ = nullptr;
    repeat_ = 0;
  }

  std::string toString() {
    std::stringstream ss;
    ss << "timer info:\t id: " << timer_id_;
    ss << ", interval: " << interval_;
    ss << ", repeat: " << repeat_;
    return ss.str();
  }

private:
  void Trigger() {
    if (cb_) {
      cb_();
    }

    --repeat_;
  }

private:
  friend class TimerMgr;
  int32_t timer_id_;
  int32_t interval_;
  int32_t repeat_;
  int64_t time_out_;
  std::function<void()> cb_;
};

class TimerComparer {
public:
  bool operator()(Timer::Ptr& lhs, Timer::Ptr& rhs) {
    return lhs->GetLeftTime() > rhs->GetLeftTime();
  }
};

class TimerMgr {
public:
  int32_t AddTimer(int32_t interval, std::function<void()>&& cb,
      int32_t delay = 0, int32_t repeat = 1) {

    auto timer = std::make_shared<Timer>(interval,
      std::forward<std::function<void()>>(cb), delay, repeat);

    AddTimer(timer);
    return timer->timer_id_;
  }

  void DelTimer(int32_t id) {
    auto timer = FindTimer(id);
    if (timer) {
      timer->Cancel();
    }
  }

  void Schedule() {
    std::list<Timer::Ptr> trigger;

    while (!timers_.empty()) {
      auto timer = timers_.top();
      if(timer->GetLeftTime() > 0)
        break;

      timer->Execute();

      // 触发后的定时器需要重新加入
      trigger.push_back(timer);
      // 删除触发的定时器
      timers_.pop();
    }

    for (auto&& timer : trigger) {
      SetupTimer(timer);
    }
  }

private:
  Timer::Ptr FindTimer(int32_t id) {
    auto itr = timers_dict_.find(id);
    return itr != timers_dict_.end() ? itr->second : nullptr;
  }

  void AddTimer(Timer::Ptr timer) {
    timers_.emplace(timer);
    timers_dict_[timer->timer_id_] = timer;
  }

  void SetupTimer(Timer::Ptr timer) {
    if (timer->repeat_ > 0) {
      timer->Start();
      timers_.emplace(timer);
    }
    else {
      timers_dict_.erase(timer->timer_id_);
    }
  }

private:
  std::priority_queue<Timer::Ptr, std::vector<Timer::Ptr>, TimerComparer> timers_;
  std::unordered_map<int32_t, Timer::Ptr> timers_dict_;
};
