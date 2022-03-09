#ifndef __SORT_H__
#define __SORT_H__

#include <string>

// shuffle array a.
void shuffle(int a[], int len);

// random value[from, to].
int randInt(int from, int to);

void insertSort(int a[], int len);

void bubbleSort(int a[], int len);

void shellSort(int a[], int len);

void quickSort(int a[], int len);

void heapSort(int a[], int len);

void mergeSort(int a[], int len);

// after quickSelect, a[k-1] is the top-k value.
void quickSelect(int a[], int len, int k);

int topK(int a[], int len, int k);

// 连续子数组的最大和( 动态规划问题dp(i) = max(dp(i-1)+a[i], a[i]) )
int maxSumOfSubArray(int a[], int len, int* left, int* right);

// 查找不重复字符的最大连续子串的长度
int findMaxSubStrLength(const std::string &in, int &start);
int lengthOfLongestSubstring(std::string &str, int &from);

#endif // __SORT_H__
