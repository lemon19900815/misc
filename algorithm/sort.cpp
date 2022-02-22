#include "sort.h"

void swap(int& a, int& b)
{
	int c = a;
	a = b;
	b = c;
}

// 降序（前面是排序好的）
void insertSort(int arr[], int len)
{
    int j;
    for(int i = 1; i < len; ++i)
    {
        auto tmp = arr[i];
        // 上一轮：0~j-1已经是排好序的
        for(j = i; j > 0 && tmp < arr[j-1]; --j)
            arr[j] = arr[j-1];
        arr[j] = tmp;
    }
}

// 降序（后面是排序的）
void bubbleSort(int arr[], int len)
{
    for(int i = 0; i < len; ++i)
    {
        // 最后的是排好序的
        for(int j = 0; j+1 < len-i; --j)
            if(arr[j] > arr[j+1]) swap(arr[j], arr[j+1]);
    }
}
