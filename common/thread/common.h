#ifndef _Common_H_
#define _Common_H_

#if defined(linux) || defined(__linux) || defined(__linux__)
#   ifndef PLATFORM_LINUX
#       define PLATFORM_LINUX
#   endif
#elif defined(WIN32) || defined(_WIN32) || defined(_WIN64)
#   ifndef PLATFORM_WINDOWS
#       define PLATFORM_WINDOWS
#   endif
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#   ifndef PLATFORM_MACOS
#       define PLATFORM_MACOS
#   endif
#else
#   error Unsupported platform.
#endif

#if defined(_MSC_VER)
#   ifndef COMPILER_MSVC
#       define COMPILER_MSVC
#   endif
#elif defined(__GNUC__)
#   ifndef COMPILER_GCC
#       define COMPILER_GCC
#   endif
#endif

#if defined (PLATFORM_WINDOWS)
#include <windows.h>
#pragma warning(disable: 4996)
#endif

#if defined(COMPILER_MSVC)
typedef UINT8 uint8_t;
typedef UINT16 uint16_t;
typedef UINT32 uint32_t;
typedef UINT64 uint64_t;
typedef INT8 int8_t;
typedef INT16 int16_t;
typedef INT32 int32_t;
typedef INT64 int64_t;
#else
#include <stdint.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <vector>

// log message print to call layer
typedef void (*PLogMessage)(const char* message);

#endif // _Common_H_
