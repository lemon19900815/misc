#pragma once

#include <stdio.h>

#include <iostream>
#include <cstring>
#include <string>
#include <thread>

#define Safe_Delete(ptr) if((ptr)) { delete (ptr); (ptr) = nullptr; }
#define Safe_Delete_Array(ptr) if((ptr)) { delete [] (ptr); (ptr) = nullptr; }

#define Virtual_Destructor(cls) virtual ~cls() { }

#define CLASS_PTR(cls)\
    using Ptr = std::shared_ptr<cls>;\
    using Wptr = std::weak_ptr<cls>;

//#define CALSS_CREATOR(cls, ...)\
//    std::make_shared<cls>(...);

#define LOG(str) std::cout << str << std::endl;

static void SleepMs(int32_t ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
}
