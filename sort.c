#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

//takes an input int array and builds a max heap from that.
void heapify(int arr[], int n, int root){
	int max = root;

	int left = 2 * root + 1;
	int right = 2 * root + 2;

 	if (left < n && arr[left] > arr[max]){
        max = left;
	}
    
	if (right < n && arr[right] > arr[max]){
        max = right;
	}
    
    if (max != root) {
		int temp = arr[root];
		arr[root]= arr[max];
		arr[max] = temp;
        heapify(arr, n, root);
    }
}
// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	for(int i = n/2-1; i >= 0; i--){
		//builds a max heap
		heapify(arr, n, i);
	}
	for(int i = n - 1; i >= 0; i--){
		
		//swaps first element with the last one
		int temp = arr[i];
		arr[i] = arr[0];
		arr[0] = arr[i];

		//creates a max heap out of the remaining values 
		heapify(arr,n,i);
	}
}

void merge(int arr[], int l, int m, int r){
	//variables to be used later
	int i, j, k;

	//length of the left half (middle val - min value) +1 (0 index)
	int n1 = m - l + 1;
	//length of right half (max value - middle value)
	int n2 = r-m;

	//allocates extra memory for temparay arrays to merge
	int *left = (int*) malloc(n1*sizeof(int));
	extraMemoryAllocated++;
	int *right = (int*)malloc(n2*sizeof(int));
	extraMemoryAllocated++;
	
	//copies data from the inputed array from min value to the length of the left half.
	for(i = 0; i < n1; i++){
		left[i] = arr[l +i ];
	}
	//copies data from the inputed array from the length of the left half to the max value.
	for(j = 0; j < n2; j++){
		right[j] = arr[j + m + 1];
	}

	//merges temp arrays back together

	i = 0;
	j = 0;
	k = l; //first index of the orginial array/merged array

	while(i < n1 &&j < n2){
		if(left[i]<right[j]){
			arr[k] = left[i];
			i++;
		}
		else{
			arr[k] = right[j];
			j++;
		}
		k++;
	}

	//copies the remaining elements in the temp array
	for(i;i<n1;i++){
		arr[k] = left[i];
		k++;
	}
	for(j;j<n2;j++){
		arr[k] = right[j];
		k++;
	}

	//frees the temporarily allocated memory
	free(left);
	free(right);
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	//checks if left is less than right, else do nothing 
	if(l < r){

		//find midpoint
		int m = (l + r)/2;

		//recursively sort left half
		mergeSort(pData,l,m);
		//recursively sort right half
		mergeSort(pData,m+1,r);

		//merges the two halves
		merge(pData,l,m,r);
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
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
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
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
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}