//ref: CLRS $6

#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <assert.h> 
#include <algorithm> 

#define MAX 20

int number[MAX+1];
int heap_size;

void printHeap()
{
	for(int i = 1; i <= heap_size; i++) { 
        printf("%d ", number[i]); 
    } 
	printf("\n");
}

int parentId( int i) 
{
	return i/2;
}

int leftId( int i) 
{
	return i*2;
}

int rightId(int i) 
{
	return i*2+1;
}


//from A[i]->leaf, to make A[i] at the correct position
void maxHeapify(int i) 
{
	int l = leftId(i);
	int r = rightId(i);
	
	int largest;
	// if leftId child is larger than current, then 
	if( l<=heap_size && number[l]>number[i] )
		largest = l;
	else
		largest = i;
		
	// if rightId child is larger than current, then 
	if( r<=heap_size && number[r]>number[largest] )
		largest = r;
	
	if( largest!=i )	
	{
		//swap and 
		std::swap( number[i], number[largest] );
		maxHeapify(largest);
	}
}

void buildMaxHeap() { 
    for(int i=heap_size/2; i>=1; i--)
	{
		maxHeapify(i);
	}
} 

void heapSort() { 
    buildMaxHeap();
	printf("\ncreate heap:"); 
	printHeap();
	
	int heap_size_ = heap_size;
	for(int i=heap_size; i>=2; i--)
	{
		std::swap(number[1], number[i]);
		heap_size--;
		maxHeapify(1);
	}
	heap_size = heap_size_;
} 

// priority queue
int heapMaximum()
{
	return number[1];
}

int heapExtractMax()
{
	assert(heap_size>=1);

	int max = number[1];

	//move the last value to root, then adjust to heap
	number[1] = number[heap_size];
	heap_size --;
	maxHeapify(1);

	return max;
}

// update number[i] to key
void heapIncreaseKey(int i, int key)
{
	assert(key>=number[i]);

	//adjust from [i] to root
	number[i] = key;
	while( i>1 && number[parentId(i)]<number[i] )
	{
		std::swap( number[parentId(i)], number[i] );
		i  = parentId(i);
	}
}

#define	MINIMUM_INT		-65555

void maxHeapInsert(int key)
{
	heap_size ++;
	number[heap_size] = MINIMUM_INT;
	heapIncreaseKey( heap_size, key );
}

int main(void) { 
 //   srand(1); 
 //   
 //   //init the data
 //   int i;
 //   for(i = 1; i <= MAX; i++) { 
 //       number[i] = rand() % 100; 
 //   } 
	//heap_size = MAX;
	
	printf("test heapSort\n");
	int a[] = {4,1,3,2,16,9,10,14,8,7};
	heap_size = sizeof(a)/sizeof(a[0]);
	std::copy(a, a+heap_size,number+1);
	
	// print
	printf("before sort:"); 
	printHeap();
	
	// sort
    heapSort(); 
    printf("after sort"); 
	printHeap();

	printf("test priority queue\n");
	heap_size = sizeof(a)/sizeof(a[0]);
	std::copy(a, a+heap_size,number+1);

	buildMaxHeap();
	heapIncreaseKey( 9, 15 );
	printHeap();

    return 0; 
} 
