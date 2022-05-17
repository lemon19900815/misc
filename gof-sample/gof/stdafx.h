#pragma once

#include <stdio.h>

#include <iostream>
#include <cstring>
#include <string>

#define Safe_Delete(ptr) if((ptr)) { delete (ptr); (ptr) = nullptr; }
#define Safe_Delete_Array(ptr) if((ptr)) { delete [] (ptr); (ptr) = nullptr; }

#define Virtual_Destructor(cls) virtual ~cls() { }

#define CLASS_PTR(cls)\
    using Ptr = std::shared_ptr<cls>;\
    using Wptr = std::weak_ptr<cls>;

//#define CALSS_CREATOR(cls, ...)\
//    std::make_shared<cls>(...);
