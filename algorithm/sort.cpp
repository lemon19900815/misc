#include "sort.h"

#include <cstdlib>
#include <cassert>

static void swap(int& a, int& b)
{
	int c = a;
	a = b;
	b = c;
}

// random value[from, to].
int randInt(int from, int to)
{
	assert(to >= from);
	return rand()%(to-from+1)+from;
}

void shuffle(int a[], int len)
{
	int i = 0;
	for(; i < len; ++i)
	{
		int idx = randInt(0, len-i-1);
		swap(a[idx], a[len-i-1]);
	}
}

// 降序（前面是排序好的）
void insertSort(int a[], int len)
{
    int j;
    for(int i = 1; i < len; ++i)
    {
        auto tmp = a[i];
        // 上一轮：0~j-1已经是排好序的
        for(j = i; j > 0 && tmp < a[j-1]; --j)
            a[j] = a[j-1];
        a[j] = tmp;
    }
}

// 降序（后面是排序的）
void bubbleSort(int a[], int len)
{
    for(int i = 0; i < len; ++i)
    {
        // 最后的是排好序的
        for(int j = 0; j+1 < len-i; ++j)
            if(a[j] > a[j+1]) swap(a[j], a[j+1]);
    }
}

void shellSort(int a[], int len)
{
	int j;
	//for (int gap = len/2; gap > 0; gap /= 2)
	for (int gap = len>>1; gap > 0; gap = gap>>1)
	{
		for(int i = gap; i < len; ++i)
		{
			auto tmp = a[i];
			for (j = i; j >= gap && tmp < a[j - gap]; j -= gap)
				a[j] = a[j-gap];
			a[j] = tmp;
		}
	}
}

// return median of left, center, right.
// order these and hide pivot.
int median3(int a[], int left, int right)
{
	int center = (left+right)/2;
	if(a[center] < a[left])
		swap(a[center], a[left]);
	if(a[right] < a[center])
		swap(a[right], a[center]);
	if(a[right] < a[left])
		swap(a[right], a[left]);
	// hide pivot.
	swap(a[center], a[right-1]);
	return a[right-1];
}

// internal quicksort method that makes recursive calls.
// uses median-of-three partioning ---- not use. ( and a cutoff of 10.)
// notice: use a cutoff of 10, open the marked code, and mark if(left>=right)
void quickSort(int a[], int left, int right)
{
	//if(left + 10 < right)
	//{
	//	insertSort(a+left, right-left+1);
	//	return;
	//}

	if(left >= right)
		return;

	// median3避免最坏情况发生 O(n^2)
	int pivot = median3(a, left, right);
	int i = left, j = right - 1;

	for(;;)
	{
		while(a[++i] < pivot);
		while(a[--j] >= pivot);

		if(i < j)
			swap(a[i], a[j]);
		else
			break;
	}

	// 不要使用该模块，否则如果a[i]==a[j]会造成死循环
	// i = left+1,j=right-2;

	// for (;;)
	// {
	// 	while (a[i] < pivot) ++i;
	// 	while (a[j] >= pivot) --j;

	// 	if (i < j)
	// 		swap(a[i], a[j]);
	// 	else
	// 		break;
	// }

	// restore pivot
	swap(a[i], a[right-1]);
	
	quickSort(a, left, i-1);
	quickSort(a, i+1, right);
}

void quickSort(int a[], int len)
{
	quickSort(a, 0, len-1);
}

// 堆的左孩子节点
inline int leftChild(int i)
{
	return 2*i;
}

// 下滤操作有时也叫做shitDown
void percolateDown(int a[], int i, int n)
{
	int child;
	int tmp;

	for(tmp = a[i]; leftChild(i) < n; i = child)
	{
		child = leftChild(i);
		if(child != n-1 && a[child] < a[child+1])
			child++;
		if(tmp < a[child])
			a[i] = a[child];
		else
			break;
	}
	a[i] = tmp;
}

void buildHeap(int a[], int len)
{
	// 注意，此时我们的堆是从0开始索引的
	for (int i = len / 2; i >= 0; --i)
		percolateDown(a, i, len);
}

void heapSort(int a[], int len)
{
	// build heap.
	buildHeap(a, len);

	for (int j = len - 1; j > 0; --j)
	{
		swap(a[0], a[j]); // delete max.
		percolateDown(a, 0, j);
	}
}

// [leftPos, rightPos], [rightPos+1, rightEnd]
void merge(int a[], int tmp[], int leftPos, int rightPos, int rightEnd)
{
	int i = leftPos, j = rightPos+1;

	int tmpPos = leftPos;
	while(i <= rightPos && j <= rightEnd)
	{
		if(a[i] <= a[j])
			tmp[tmpPos++] = a[i++];
		else
			tmp[tmpPos++] = a[j++];
	}

	// copy the rest of left half.
	while(i <= rightPos)
		tmp[tmpPos++] = a[i++];

	// copy the rest of right half.
	while(j <= rightEnd)
		tmp[tmpPos++] = a[j++];

	// copy tmp array back.
	while (leftPos <= rightEnd)
	{
		a[leftPos] = tmp[leftPos];
		++leftPos;
	}
}

void mergeSort(int a[], int tmp[], int left, int right)
{
	if(left >= right)
		return;

	int center = (left+right)/2;
	mergeSort(a, tmp, left, center);
	mergeSort(a, tmp, center+1, right);

	merge(a, tmp, left, center, right);
}

void mergeSort(int a[], int len)
{
	int* tmp = new int[len];
	mergeSort(a, tmp, 0, len-1);

	delete[] tmp;
	tmp = nullptr;
}

// after quickSelect, a[k-1] is the top-k value.
void quickSelect(int a[], int left, int right, int k)
{
	if (left >= right)
		return;

	int pivot = median3(a, left, right);
	int i = left, j = right - 1;

	for (;;)
	{
		while (a[++i] < pivot);
		while (a[--j] >= pivot);

		if (i < j)
			swap(a[i], a[j]);
		else
			break;
	}

	// restore pivot
	swap(a[i], a[right - 1]);

	if(i > k)
		quickSelect(a, left, i - 1, k);
	else
		quickSelect(a, i + 1, right, k);
}

void quickSelect(int a[], int len, int k)
{
	quickSelect(a, 0, len-1, k);
}

int topK(int a[], int len, int k)
{
	assert(k > 0 && k <= len);
	quickSelect(a, len, k);
	return a[k-1];
}