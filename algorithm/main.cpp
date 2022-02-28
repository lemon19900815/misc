#include "sort.h"

#include <iostream>
#include <sstream>
#include <string>
#include <functional>
#include <ctime>
#include <unordered_map>
#include <vector>

// c++11
#define BIND_FUNC(Name) std::bind(&Name, std::placeholders::_1, std::placeholders::_2)

void print(std::string prefix, int arr[], int len)
{
	std::stringstream ss;
	ss << prefix << ": ";
	for(int i = 0; i < len; ++i)
		ss << arr[i] << ", ";
	std::cout << ss.str() << std::endl;
}

void test(std::string name, std::function<void(int arr[], int len)> sortFunc, int arr[], int len)
{
	print("before " + name, arr, len);

	sortFunc(arr, len);
	print("after " + name, arr, len);
	std::cout << "===================================" << std::endl;
}

void reset(int src[], int dest[], int len)
{
	for(int i = 0; i < len; ++i)
		dest[i] = src[i];
}

int main(int argc, char* argv[])
{
	srand(time(nullptr));
	
	std::unordered_map<std::string, std::function<void(int arr[], int len)>> sortFuncs;
	sortFuncs["bubble"] = BIND_FUNC(bubbleSort);
	sortFuncs["insert"] = BIND_FUNC(insertSort);
	sortFuncs["quick"] = BIND_FUNC(quickSort);
	
	
	// test cases
	int cases[][7] = {
		{1, 5, 3, 10, 2, 9, 4},
		{4, 4, 4, 4, 7, 4, 4},
		{4, 4, 4, 4, 4, 4, 4},
	};
	
	for(int i = 0; i < sizeof(cases)/sizeof(cases[0]); ++i)
	{
		int len = 7;
		int* dest = new int[len];
		for(auto& fn: sortFuncs)
		{
			reset(cases[i], dest, len);
			test(fn.first, fn.second, dest, len);
		}
		delete[] dest;
		dest = nullptr;
	}

	return 0;
}