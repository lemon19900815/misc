#ifndef __TimerHelper_Inc_H__
#define __TimerHelper_Inc_H__

#include "timer.h"
#include "singleton.h"


class TimerHelper : public singleton<TimerHelper>
{
public:
  TimerHelper();
  ~TimerHelper() override;

  /*
    start up timer with current time ms
  */
  void StartTimer(uint64_t ms);

  /*
    stop timer
  */
  void StopTimer();

  /*
    add a timer by peroid(ms), timer execute callback function with data
  */
  uint64_t AddTimer(uint64_t period, std::function<void(uint64_t)> fn, uint64_t data);

  /*
    del the timer with timerid
  */
  int32_t DelTimer(uint64_t timerid);

  /*
    del the timer
  */
  int32_t DelTimer(Timer::PTR timer);

  /*
    get the timer with timerid
  */
  Timer::PTR GetTimer(uint64_t timerid);

  /*
    update timer
  */
  void Update(uint32_t delta);

private:
  inline uint64_t genTimerId() {
    static uint64_t id_ = 0;
    return ++id_;
  }

private:
  Wheel wheel_;
  std::unordered_map<uint64_t, Timer::WPTR> timers_;
};

#endif // __TimerHelper_Inc_H__
