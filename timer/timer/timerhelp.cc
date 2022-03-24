#include "timerhelp.h"

TimerHelper::TimerHelper()
{

}

TimerHelper::~TimerHelper()
{
  stop_timer(&wheel_);
}

/*
  start up timer with current time ms
*/
void TimerHelper::StartTimer(uint64_t ms)
{
  init_timer(&wheel_, ms);
}

/*
  stop timer
*/
void TimerHelper::StopTimer()
{
  stop_timer(&wheel_);
}

/*
  add a timer by peroid(ms), timer execute callback function with data
*/
uint64_t TimerHelper::AddTimer(
  uint64_t period, std::function<void(uint64_t)> fn, uint64_t data)
{
  // TODO: peroid accuracy is 50ms
  if (period % 50 != 0) {
    printf("[WARNING] timer accuracy is 50ms\n");
  }

  auto timer = std::make_shared<Timer>();
  timer->expires = wheel_.ms /*+ wheel_.deltams*/ + period;
  timer->cb = fn;
  timer->data = data;

  if (0 == add_timer(&wheel_, timer))
  {
    timer->timerid = genTimerId();
    timers_[timer->timerid] = timer;

    return timer->timerid;
  }

  return 0;
}

/*
  del the timer with timerid
*/
int32_t TimerHelper::DelTimer(uint64_t timerid)
{
  return DelTimer(GetTimer(timerid));
}

/*
  del the timer
*/
int32_t TimerHelper::DelTimer(Timer::PTR timer)
{
  if (!timer)
    return -1;

  del_timer(&wheel_, timer);

  timers_.erase(timer->timerid);

  return 0;
}

/*
  get the timer with timerid
*/
Timer::PTR TimerHelper::GetTimer(uint64_t timerid)
{
  auto it = timers_.find(timerid);
  return it != timers_.end() ? it->second.lock() : nullptr;
}

/*
  update timer
*/
void TimerHelper::Update(uint32_t delta)
{
  tick_timer(&wheel_, delta);
}
