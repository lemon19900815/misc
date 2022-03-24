#include "timer.h"

#include <stdio.h>

constexpr uint32_t kDefaultInterval = 50;

constexpr uint64_t kOneSecondMs = 1000;
constexpr uint64_t kOneMinuteMs = 60 * kOneSecondMs;
constexpr uint64_t kOneHourMs = 60 * kOneMinuteMs;
constexpr uint64_t kOneDayMs = 24 * kOneHourMs;
constexpr uint64_t kOneWeekMs = 7 * kOneDayMs;

/*
  init timer with current milliseconds and interval(ms)
*/
bool init_timer(Wheel* wheel, uint64_t now/*, uint32_t interval = 50*/)
{
  // 50ms, 1s, 1m, 1h, 1d
  static uint32_t buckets_number[] = { 20, 60, 60, 24, 7 };

  for(uint32_t i = 0; i < kTimerHierarchyNumber; ++i)
  {
    auto& hbuckets = wheel->hbuckets[i];
    hbuckets.bucketlen = buckets_number[i];
    hbuckets.bucketcursor = 0;
    hbuckets.bucketptr = new Bucket[hbuckets.bucketlen];
    //memset(hbuckets.bucketptr, 0, sizeof(Bucket) * hbuckets.bucketlen);
  }

  wheel->ms = now;
  wheel->deltams = 0;
  wheel->interval = kDefaultInterval; // 50ms for game timer, 50ms is suitable enough

  return true;
}

/*
  stop timer
*/
void stop_timer(Wheel* wheel)
{
  for (uint32_t i = 0; i < kTimerHierarchyNumber; ++i)
  {
    if (wheel->hbuckets[i].bucketptr) {
      delete[] wheel->hbuckets[i].bucketptr;
      wheel->hbuckets[i].bucketptr = nullptr;
    }
  }
}

static bool calc_bucket_position(
  Wheel* wheel, Timer::PTR timer,
  uint32_t& hierarchy_index, uint32_t& bucket_index)
{
  uint64_t expires = wheel->ms >= timer->expires ? 0 : timer->expires - wheel->ms;

  if(expires < kOneSecondMs) {
    hierarchy_index = 0;
    bucket_index = expires / wheel->interval;
  }
  else if(expires < kOneMinuteMs) {
    hierarchy_index = 1;
    bucket_index = expires / kOneSecondMs;
  }
  else if(expires < kOneHourMs) {
    hierarchy_index = 2;
    bucket_index = expires / kOneMinuteMs;
  }
  else if(expires < kOneDayMs) {
    hierarchy_index = 3;
    bucket_index = expires / kOneHourMs;
  }
  else if(expires < kOneWeekMs) {
    hierarchy_index = 4;
    bucket_index = expires / kOneDayMs;
  }
  else {
    return false;
  }

  if(bucket_index >= wheel->hbuckets[hierarchy_index].bucketlen) {
    printf("bucket index error[%u]\n", bucket_index);
    return false;
  }

  bucket_index = (bucket_index + wheel->hbuckets[hierarchy_index].bucketcursor) % wheel->hbuckets[hierarchy_index].bucketlen;

  return true;
}

/*
  add a timer to wheel manager, return with 0 success, -1 max timer time limit(max support one week)
*/
int32_t add_timer(Wheel* wheel, Timer::PTR timer)
{
  uint32_t hierarchy_index = 0;
  uint32_t bucket_index = 0;

  if(!calc_bucket_position(wheel, timer, hierarchy_index, bucket_index))
    return -1;

  auto bucket = wheel->hbuckets[hierarchy_index].bucketptr + bucket_index;

  bucket->timers.push_back(timer);

  timer->pos = std::prev(bucket->timers.end());
  timer->hierarychy = hierarchy_index;
  timer->bucketindex = bucket_index;

  return 0;
}

/*
  del the timer from wheel, return with 0 success, else failed
*/
int32_t del_timer(Wheel* wheel, Timer::PTR timer)
{
  auto bucket = wheel->hbuckets[timer->hierarychy].bucketptr + timer->bucketindex;

  if (!bucket->timers.empty()) {
    bucket->timers.erase(timer->pos);
    return 0;
  }

  return -1; // not found
}

/*
  timer cascade, from high hierarchy buckets to lower hierarchy buckets
*/
void timer_cascade(Wheel* wheel, Bucket* bucket)
{
  if (bucket->timers.empty())
    return;

  for(auto& it: bucket->timers) {
    add_timer(wheel, it);
  }
  bucket->timers.clear();
}

/*
  inc bucket cursor
*/
void inc_bucket_cursor(Wheel* wheel, uint32_t bucketindex)
{
  wheel->hbuckets[bucketindex].bucketcursor = (wheel->hbuckets[bucketindex].bucketcursor + 1) % wheel->hbuckets[bucketindex].bucketlen;
}

/*
  execute timer
*/
void execute_timer(Bucket* bucket)
{
  if (bucket->timers.empty())
    return;

  for(auto& it: bucket->timers)
  {
    auto timer = it;
    if(timer->cb) {
      timer->cb(timer->data);
    }
  }

  bucket->timers.clear();
}

/*
  tick timer, detal ms from last do tick event
*/
void tick_timer(Wheel* wheel, uint32_t delta)
{
  wheel->deltams += delta;

  while((int64_t)(wheel->deltams - wheel->interval) >= 0)
  {
    // cascade wheel buckets
    inc_bucket_cursor(wheel, 0);
    if(wheel->hbuckets[0].bucketcursor == 0) // ms timer buckets over
    {
      inc_bucket_cursor(wheel, 1);
      timer_cascade(wheel, wheel->hbuckets[1].bucketptr + wheel->hbuckets[1].bucketcursor);
      if(wheel->hbuckets[1].bucketcursor == 0) // seconds timer buckets over
      {
        inc_bucket_cursor(wheel, 2);
        timer_cascade(wheel, wheel->hbuckets[2].bucketptr + wheel->hbuckets[2].bucketcursor);
        if(wheel->hbuckets[2].bucketcursor == 0) // minute timer buckets over
        {
          inc_bucket_cursor(wheel, 3);
          timer_cascade(wheel, wheel->hbuckets[3].bucketptr + wheel->hbuckets[3].bucketcursor);
          if(wheel->hbuckets[3].bucketcursor == 0) // hour timer buckets over
          {
            inc_bucket_cursor(wheel, 4);
            timer_cascade(wheel, wheel->hbuckets[4].bucketptr + wheel->hbuckets[4].bucketcursor);
            if(wheel->hbuckets[4].bucketcursor == 0) // day timer buckets over
            {
              perror("only support one week timer\n");
            }
          }
        }
      }
    }

    execute_timer(wheel->hbuckets[0].bucketptr + wheel->hbuckets[0].bucketcursor);

    wheel->deltams -= wheel->interval;
    wheel->ms += wheel->interval;
  }
}
