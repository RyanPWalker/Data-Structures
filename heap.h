/***********************************************************************
* Program:
*    Week 11, Sorting
*    Brother Ercanbrack, CS 235
*
* Author:
*   Ryan Walker
* Summary: 
*	 This is the implementation of a heap sort.  It will receive a vector
* and sort its contents similar to a binary search tree.  While percolating
* down is a method for doing a heap sort, I found it much easier to have
* it percolate recursively.
************************************************************************/
#include <vector>

using namespace std;

/*************************************************************************
* heapify
* This function is recursive and will sort a vector(or potentially an array).
* It places the largest value at the end of the vector and continues to swap
* values placing the next largest value at the front of the sorted section
* of the array.
**************************************************************************/
template<class T>
void heapify(vector<T> & data, int num, int i)
{
	int largest    = i;
	int leftChild  = 2 * i + 1;
	int rightChild = 2 * i + 2;

	// is the left child greater?
	if (leftChild < num && data[leftChild] > data[largest])
		largest = leftChild;

	// or is the right child greater?
	if (rightChild < num && data[rightChild] > data[largest])
		largest = rightChild;
    
	if (largest != i)
	{
		// swap places, putting the next largest value at the
		// front of the sorted section of the array
		swap(data[i], data[largest]);
		// go again
		heapify(data, num, largest);
	}
}

/*************************************************************************
* heapSort
* This function sorts a vector using a heap sort.
* Input:  data -  Vector to be sorted.
* Output: data -  Vector sorted
**************************************************************************/
template<class T>
void heapSort(vector<T> &data)
{
	for (int i = data.size() / 2 - 1; i >= 0; i--)
	{
		// heapifys the data
		heapify(data, data.size(), i);
	}

	for (int i = data.size() - 1; i >= 0; i--)
	{
		// starts at the end of the heap and works its way up
   	swap(data[0], data[i]);
   	heapify(data, i, 0);
	}
}