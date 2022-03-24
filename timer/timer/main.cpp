#include <stdio.h>
#include <chrono>
#include <thread>

#include "timerhelp.h"

uint64_t now()
{
  return std::chrono::system_clock::now().time_since_epoch().count() / 10000;
}

static uint32_t times = 1;

void ontimer(uint64_t data)
{
  printf("===================ontimer[%llu]======================\n", data);

  TimerHelper::instance()->AddTimer(50 * times, std::bind(&ontimer, std::placeholders::_1), times);
  ++times;
}

void deltimer(uint64_t val)
{
  printf("del timer[%llu]\n", val);
  TimerHelper::instance()->DelTimer(val);

  auto tmp = TimerHelper::instance()->GetTimer(val);
  if (!tmp)
  {
    printf("deleted.\n");
  }
}

void addtimer()
{
  uint64_t timerid = TimerHelper::instance()->AddTimer(5000, std::bind(&ontimer, std::placeholders::_1), 100);
  TimerHelper::instance()->AddTimer(500, std::bind(&deltimer, std::placeholders::_1), timerid);
}

int main(int argc, char* argv[])
{
  uint64_t last = 0;
  uint64_t current = now();

  uint32_t epoch_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  TimerHelper::instance()->StartTimer(current);

  addtimer();

  while (true)
  {
    last = current;
    current = now();

    TimerHelper::instance()->Update(current - last);

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }

  TimerHelper::instance()->StopTimer();

  return 0;
}

