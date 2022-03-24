#ifndef __COMMON_INC_H__
#define __COMMON_INC_H__

#define SAFE_DELETE(p) if((p)) delete (p); (p) = nullptr;
#define SAFE_DELETE_ARRAY(p) if((p)) delete [] (p); (p) = nullptr;

#define NOTUSED(a) (void)a

#include <cstdint>
#include <cstring>
#include <iostream>

#include <chrono>
#include <ctime>

#define SAFE_COPY(dest, src, len) strncpy(dest, src, len); dest[len - 1] = '\0';

#ifdef _WIN32
#pragma warning(disable: 4996)
#endif

#endif // !__COMMON_INC_H__
