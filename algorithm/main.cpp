#include "sort.h"

#include <iostream>
#include <sstream>
#include <string>
#include <functional>
#include <ctime>
#include <unordered_map>
#include <vector>

//#define LOG_RESULT

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
	sortFunc(arr, len);

#ifdef LOG_RESULT
	print("after " + name, arr, len);
	std::cout << "===================================" << std::endl;
#endif
}

void reset(int src[], int dest[], int len)
{
	for(int i = 0; i < len; ++i)
		dest[i] = src[i];
}

bool check(int a[], int b[], int len)
{
	for(int i = 0; i < len; ++i)
	{
		if(a[i] != b[i])
			return false;
	}
	return true;
}

int main(int argc, char* argv[])
{
	srand(time(nullptr));

	std::unordered_map<std::string, std::function<void(int arr[], int len)>> sortFuncs;
	sortFuncs["bubble"] = BIND_FUNC(bubbleSort);
	sortFuncs["insert"] = BIND_FUNC(insertSort);
	sortFuncs["quick"] = BIND_FUNC(quickSort);
	sortFuncs["heap"] = BIND_FUNC(heapSort);

	// test cases
	int cases[][7] = {
		{1, 5, 3, 10, 2, 9, 4},
		{4, 4, 4, 4, 7, 4, 4},
		{4, 4, 5, 4, 8, 4, 5},
	};

	int ordered_cases[][7] = {
		{1, 2, 3, 4, 5, 9, 10},
		{4, 4, 4, 4, 4, 4, 7},
		{4, 4, 4, 4, 5, 5, 8},
	};

	for(int i = 0; i < sizeof(cases)/sizeof(cases[0]); ++i)
	{
		int len = 7;

		print("case" + std::to_string(i), cases[i], len);
		print("ordered case" + std::to_string(i), ordered_cases[i], len);

		int* dest = new int[len];
		for(auto& it: sortFuncs)
		{
			auto& name = it.first;
			auto& sortFn = it.second;

			reset(cases[i], dest, len);
			test(name, sortFn, dest, len);

			if (!check(dest, ordered_cases[i], len))
				std::cout << name << " -> fail." << std::endl;
			else
				std::cout << name << " -> success." << std::endl;
		}
		delete[] dest;
		dest = nullptr;

		std::cout << "===============================================" << std::endl;
	}

	int arr[] = {30, 20, 5, 3, 20, 10, 99};
	// sorted: {3, 5, 10, 20, 20, 30, 99};
	print("before select", arr, sizeof(arr)/sizeof(arr[0]));
	std::cout << "top-3th: " << topK(arr, sizeof(arr)/sizeof(arr[0]), 3) << std::endl;

	return 0;
}