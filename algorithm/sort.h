#ifndef __SORT_H__
#define __SORT_H__

// shuffle array a.
void shuffle(int a[], int len);

// random value[from, to].
int randInt(int from, int to);

void insertSort(int a[], int len);

void bubbleSort(int a[], int len);

void shellSort(int a[], int len);

void quickSort(int a[], int len);

void heapSort(int a[], int len);

// after quickSelect, a[k-1] is the top-k value.
void quickSelect(int a[], int len, int k);

int topK(int a[], int len, int k);

#endif // __SORT_H__
