#include "datetime.h"
#include <ctime>
#include <cassert>

/*
* default constructor with system time
*/
DateTime::DateTime()
{
  unixstamp_ = time(0);
  tm_ = localtime(&unixstamp_);
}

/*
* constructor with unixstamp
*/
DateTime::DateTime(unsigned int unixstamp)
{
  unixstamp_ = unixstamp;
  tm_ = localtime(&unixstamp_);
  assert(tm_);
}

/*
* constructor with year/month/day hour/min/sec
*/
DateTime::DateTime(int year, int month, int day,
  int hour, int min, int sec)
{
  assert(year >= 1900);
  assert(month > 0 && month <= 12);
  assert(day > 0 && day <= 31);
  assert(hour >= 0 && hour < 24);
  assert(min >= 0 && min < 60);
  assert(sec >= 0 && sec < 60);

  tm tmp;
  tmp.tm_year = year - 1900;
  tmp.tm_mon = month - 1;
  tmp.tm_mday = day;
  tmp.tm_hour = hour;
  tmp.tm_min = min;
  tmp.tm_sec = sec;

  unixstamp_ = mktime(&tmp);
  tm_ = localtime(&unixstamp_);
}

/*
* current datetime's unixstamp
*/
unsigned DateTime::unixstamp()
{
  return unixstamp_;
}

/*
* add year on current datetime, success return true, otherwise false
*/
bool DateTime::addYear(int year)
{
  tm tmp = *tm_;
  tmp.tm_year += year;
  time_t ret = mktime(&tmp);
  if (ret == (time_t)-1)
    return false;

  unixstamp_ = ret;
  tm_ = localtime(&unixstamp_);

  return true;
}

/*
* add day on current datetime, success return true, otherwise false
*/
bool DateTime::addDay(int day)
{
  time_t t = unixstamp_ + day * kOneDaySec;

  tm* tmp = localtime(&t);
  if (!tmp) return false;

  unixstamp_ = t;
  tm_ = tmp;

  return true;
}

/*
* add hour on current datetime, success return true, otherwise false
*/
bool DateTime::addHour(int hour)
{
  time_t t = unixstamp_ + hour * kOneHourSec;

  tm* tmp = localtime(&t);
  if (!tmp) return false;

  unixstamp_ = t;
  tm_ = tmp;

  return true;
}

/*
* add minute on current datetime, success return true, otherwise false
*/
bool DateTime::addMinute(int min)
{
  time_t t = unixstamp_ + min * kOneMinuteSec;

  tm* tmp = localtime(&t);
  if (!tmp) return false;

  unixstamp_ = t;
  tm_ = tmp;

  return true;
}

/*
* add second on current datetime, success return true, otherwise false
*/
bool DateTime::addSecond(int sec)
{
  time_t t = unixstamp_ + sec;

  tm* tmp = localtime(&t);
  if (!tmp) return false;

  unixstamp_ = t;
  tm_ = tmp;

  return true;
}

/*
* the year of current datetime
*/
int DateTime::year()
{
  return tm_->tm_year + 1900;
}

/*
* the month of current datetime
*/
int DateTime::month()
{
  return tm_->tm_mon + 1;
}

/*
* the day of current datetime
*/
int DateTime::day()
{
  return tm_->tm_mday;
}

/*
* the hour of current datetime
*/
int DateTime::hour()
{
  return tm_->tm_hour;
}

/*
* the minute of current datetime
*/
int DateTime::minute()
{
  return tm_->tm_min;
}

/*
* the second of current datetime
*/
int DateTime::second()
{
  return tm_->tm_sec;
}

/*
* the week day of current datetime
*/
int DateTime::wday()
{
  return tm_->tm_wday;
}

/*
* the week day of current datetime, china week of day
*/
int DateTime::cwday()
{
  int wd = wday();
  if (wd == 0) wd = 7;

  return wd;
}

/*
* as datetime to a string with format[yyyy-mm-dd hh:mm:ss]
*/
std::string DateTime::asString()
{
  char buff[256];
  snprintf(buff, sizeof(buff), "%04d-%02d-%02d %02d:%02d:%02d",
    year(), month(), day(), hour(), minute(), second());

  return std::string(buff);
}

/*
* compare the two time is different day on offset
*/
bool DateTime::diffDay(time_t t1, time_t t2, time_t offset)
{
  assert(t1 >= offset);
  assert(t2 >= offset);
  assert(offset < kOneDaySec));

  time_t delta = difftime(t1, t2);
  if (delta < kOneDaySec)
  {
    t1 -= offset;
    t2 -= offset;

    tm tm1 = *localtime(&t1);
    tm tm2 = *localtime(&t2);

    return tm1.tm_mday != tm2.tm_mday;
  }

  return true;
}

/*
* compare the two time is different week on offset
*/
bool DateTime::diffWeek(time_t t1, time_t t2, time_t offset)
{
  assert(t1 >= offset);
  assert(t2 >= offset);
  assert(offset < kOneDaySec);

  time_t delta = difftime(t1, t2);
  if (delta < kOneWeekSec)
  {
    t1 -= offset;
    t2 -= offset;

    tm tm1 = *localtime(&t1);
    tm tm2 = *localtime(&t2);

    return tm1.tm_wday != tm2.tm_wday;
  }

  return true;
}

/*
* compare the two time is different week on offset
*/
bool DateTime::diffMonth(time_t t1, time_t t2, time_t offset)
{
  assert(t1 >= offset);
  assert(t2 >= offset);
  assert(offset < kOneDaySec);

  t1 -= offset;
  t2 -= offset;

  tm tm1 = *localtime(&t1);
  tm tm2 = *localtime(&t2);

  if(tm1.tm_year == tm2.tm_year)
    return tm1.tm_mon != tm2.tm_mon;

  return true;
}

/*
* compare the two time is different year on offset
*/
bool DateTime::diffYear(time_t t1, time_t t2, time_t offset)
{
  assert(t1 >= offset);
  assert(t2 >= offset);
  assert(offset < kOneDaySec);

  t1 -= offset;
  t2 -= offset;

  tm tm1 = *localtime(&t1);
  tm tm2 = *localtime(&t2);

  return tm1.tm_year != tm2.tm_year;
}
