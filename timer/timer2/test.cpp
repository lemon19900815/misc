#include "timer.h"
#include <thread>

int main() {
  TimerMgr timer_mgr;

  std::cout << "add a timer1." << std::endl;
  timer_mgr.AddTimer(1000, []() {
    std::cout << "trigger timer1." << std::endl;
  }, 0, 10);

  std::cout << "add a timer2.";
  timer_mgr.AddTimer(5000, []() {
    std::cout << "trigger timer2." << std::endl;
  }, 0, 10);

  while (true) {
    timer_mgr.Schedule();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }

  return 0;
}
