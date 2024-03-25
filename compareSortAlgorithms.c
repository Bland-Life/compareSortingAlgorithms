#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	// printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	// printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapify(int arr[], int l, int r) {

	int parent = l;
	int left = 2 * parent + 1;
	int right = 2 * parent + 2;

	if (left < r && arr[parent] > arr[left]) {
		parent = left;
	}

	if (right < r && arr[parent] > arr[right]) {
		parent = right;
	}

	if (parent != l) {
		int temp = arr[l];
		arr[l] = arr[parent];
		arr[parent] = temp;
		heapify(arr, parent, r);
	}
}

void heapSort(int arr[], int l, int r) {

	for (int i = r; i >= l; i--) {
		heapify(arr, i, r);
	}

	for (int i = 0; i < r; i++) {
		if (2*(i+1) < r) {
			printf("%d has children %d and %d\n", arr[i], arr[2*(i+1)-1], arr[2*(i+1)]);
		}
		else if (2*(i+1) - 1 < r) {
			printf("%d has child %d", arr[i], arr[2*(i+1)-1]);
		}
	}
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r) {

	if (l < r) {
		int mid = ((l + r) / 2 + 1);
		mergeSort(pData, l, mid - 1);
		mergeSort(pData, mid, r);

		// Creates the two sub arrays for the left and right
		int* sub_arr1 = Alloc(sizeof(int*) * (mid - l));
		int* sub_arr2 = Alloc(sizeof(int*) * (r - mid + 1));

		int i;
		int j;
		i = j = 0;

		// Populates the two arrays
		for (i; i < mid - l; i++) {
			sub_arr1[i] = pData[l + i];
		}

		for (j; j <= r - mid; j++) {
			sub_arr2[j] = pData[mid + j];
		}

		i = j = 0;
		int p = l;

		// While both arrays have items, use a section of the original array as a merge array
		// Using the section, if the item of the first subarray is less than the second, insert
		// it into the appropriate position in the section of the original, otherwise insert
		// the item from the second subarray
		while (i < mid - l && j <= r - mid) {
			if (sub_arr1[i] <= sub_arr2[j]) {
				pData[p] = sub_arr1[i];
				i++;
				p++;
			}
			else {
				pData[p] = sub_arr2[j];
				j++;
				p++;
			}
		}

		// If there is an array that still has items, simply add the remaining items
		while (i < mid - l) {
			pData[p] = sub_arr1[i];
			i++;
			p++;
		}

		while (j <= r - mid) {
			pData[p] = sub_arr2[j];
			j++;
			p++;
		}

		DeAlloc(sub_arr1);
		DeAlloc(sub_arr2);
	}
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n) {

	for (int i = 0; i < n; i++) {
		int val = pData[i];
		int id = i;

		// Goes through the sorted portion of the array and pushes the elements
		// forward in the array if the current value is less than the element
		for (int j = i - 1; j >= 0; j--) {
			if (val < pData[j]) {
				pData[j + 1] = pData[j];
				id--;
			}
			else {
				break;
			}
		}
		
		pData[id] = val;
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n) {

    for (int i = 0; i < n; i++) {
        int max = pData[0];
        int id = 0;
        for (int j = 0; j < n - i; j++) {
            if (max > pData[j]) {
                pData[id] = pData[j];
                pData[j] = max;
                id = j;
            }
            else {
                max = pData[j];
                id = j;
            }
        }
    }
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n) {

    for (int i = 0; i < n; i++) {
        int min = pData[i];
        int id = i;
        for (int j = i; j < n; j++) {
            if (min > pData[j]) {
                min = pData[j];
                id = j;
            }
        }

        if (id != i) {
            pData[id] = pData[i];
            pData[i] = min;
        }
    }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);
		
		// Implement parse data block
		int* arr = *ppData;
		for (int i = 0; i < dataSz; i++) {
			fscanf(inFile, "%d \n", &arr[i]);
		}
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)Alloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

                printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
