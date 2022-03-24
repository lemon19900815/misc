#include "logger.h"
#include "timehelp.h"

#include <stdarg.h>
#include <assert.h>

Logger gLogger("log.txt");

#define LOG_BUFFER_SIZE 1024
static char buf[LOG_BUFFER_SIZE];

Logger::Logger(const char* name)
{
  fp_ = fopen(name, "w+");
}

Logger::~Logger()
{
  fclose(fp_);
}

void Logger::loginfo(const char* format, ...)
{
  va_list args;

  va_start(args, format);
  int pos = snprintf(buf, sizeof(buf), "%s [--info--] ", util::Time::asString(util::Time::now()).c_str());
  assert(pos >= 0);
  pos += vsnprintf(buf + pos, sizeof(buf) - pos, format, args);
  assert(pos >= 0);
  buf[pos] = '\n';
  buf[pos + 1] = '\0';
  printf("%s", buf);
  fprintf(fp_, buf);
  va_end(args);

  fflush(fp_);
}

void Logger::logwarn(const char* format, ...)
{
  va_list args;

  va_start(args, format);
  int pos = snprintf(buf, sizeof(buf), "%s [--warn--] ", util::Time::asString(util::Time::now()).c_str());
  assert(pos >= 0);
  pos += vsnprintf(buf + pos, sizeof(buf) - pos, format, args);
  assert(pos >= 0);
  buf[pos] = '\n';
  buf[pos + 1] = '\0';
  printf("%s", buf);
  fprintf(fp_, buf);
  va_end(args);

  fflush(fp_);
}

void Logger::logerr(const char* format, ...)
{
  va_list args;

  va_start(args, format);
  int pos = snprintf(buf, sizeof(buf), "%s [--error-] ", util::Time::asString(util::Time::now()).c_str());
  assert(pos >= 0);
  pos += vsnprintf(buf + pos, sizeof(buf) - pos, format, args);
  assert(pos >= 0);
  buf[pos] = '\n';
  buf[pos + 1] = '\0';
  printf("%s", buf);
  fprintf(fp_, buf);
  va_end(args);

  fflush(fp_);
}
