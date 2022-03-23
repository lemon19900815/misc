#pragma once

#include <stdio.h>

#include <iostream>
#include <cstring>
#include <string>

#define Safe_Delete(ptr) if((ptr)) { delete (ptr); (ptr) = nullptr; }
#define Safe_Delete_Array(ptr) if((ptr)) { delete [] (ptr); (ptr) = nullptr; }

#define Virtual_Destructor(cls) virtual ~cls() { }
