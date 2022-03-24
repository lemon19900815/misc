#ifndef __Timer_Inc_H__
#define __Timer_Inc_H__

#include <stdint.h>
#include <list>
#include <functional>
#include <unordered_map>
#include <memory>

struct Timer
{
  uint64_t timerid;
  uint64_t expires; // 过期时间

  uint64_t data;
  std::function<void(uint64_t)> cb;

  typedef std::shared_ptr<Timer> PTR;
  typedef std::weak_ptr<Timer> WPTR;

  std::list<Timer::PTR>::iterator pos;
  uint32_t hierarychy;
  uint32_t bucketindex;
};

struct Bucket
{
  std::list<Timer::PTR> timers;
};

// 层级分片bucket
struct BucketHierarchy
{
  uint32_t bucketcursor;
  uint32_t bucketlen;
  Bucket* bucketptr;
};

// ms(50ms为一个刻度,1000/50), sec(60), min(60), hour(24), day(60)
// 时间轮分片数量
constexpr uint32_t kTimerHierarchyNumber = 5;

struct Wheel
{
  uint64_t ms;
  uint32_t interval; // interval刻度--50ms
  uint64_t deltams;  // total timer delta ms

  BucketHierarchy hbuckets[kTimerHierarchyNumber];
};

/*
  init timer with current milliseconds and interval(ms)
*/
bool init_timer(Wheel* wheel, uint64_t now/*, uint32_t interval = 50*/);

/*
  stop timer
*/
void stop_timer(Wheel* wheel);

/*
  add a timer to wheel manager, return with 0 success, else failed
*/
int32_t add_timer(Wheel* wheel, Timer::PTR timer);

/*
  del the timer from wheel, return with 0 success, else failed
*/
int32_t del_timer(Wheel* wheel, Timer::PTR timer);

/*
  tick timer, detal ms from last do tick event
*/
void tick_timer(Wheel* wheel, uint32_t delta);

// 定时器精度为50ms,可以在初始化定时器设置，但是精度必须是50ms的倍数

#endif // __Timer_Inc_H__
