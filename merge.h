/***********************************************************************
* Program:
*    Week 11, Sorting
*    Brother Ercanbrack, CS 235
*
* Author:
*   Ryan Walker
* Summary: 
*	 This is the implementation of a merge sort.  mergeSort() calls split()
* and join() until the list of intergers has been successfully sorted.
************************************************************************/
#include <list>
#include <iostream>
#include <iomanip>
   
using namespace std;

// forward declarations of methods.  Helps me keep track of order.
void split(list<int> &read, list<int> &writeOne, list<int> &writeTwo, bool & done);
void join(list<int> &write, list<int> &readOne, list<int> &readTwo);
void display(list<int> &two);

/*************************************************************************
* mergeSort
* This function sorts a linked list using a Natural Merge Sort.
* Input:  data -  linked list of data to be sorted.
* Output: data -  sorted linked list
**************************************************************************/
void mergeSort(list<int> &one)
{
	list<int> two;
	list<int> three;
	bool done = false;

	// sorts till it's done
	while (!done)
	{
		split(one, two, three, done);
		if (done)
		{
			break;
		}
		join(one, two, three);
	}

	// we call display from here instead of main so that we can
	// free up data inside of this function instead of elsewhere.
	display(two);

	// Just in case, stop those memory leaks.
	one.clear();
	two.clear();
	three.clear();
}

void display(list<int> &thisList)
{
	int i = 0;
	cout << setw(3);
	for (list<int>::iterator it = thisList.begin(); it != thisList.end(); it++)
	{
		if (i != 10)
			cout << setw(3) << *it;
		if (i == 10)
		{
			cout << endl;
			cout << setw(3) << *it;
			i = 0;
		}
		i++;
	}
	cout << endl;

	thisList.clear();
}

/*************************************************************************
* split
* Split receives three lists: the first to be read from and the other two
* to be written to.  The list being read from will push onto a list until
* it finds a value less than its current value.  This creates a run.  Then
* it will swap and push to the other list until it finds a value less than
* its current value.  Then it will swap and repeat until the list is at is end.
**************************************************************************/
void split(list<int> &read, list<int> &writeOne, list<int> &writeTwo, bool & done)
{
	writeOne.clear();
	writeTwo.clear();
	bool finished = true;
	bool change = true;

	for (list<int>::iterator it = read.begin(); it != read.end(); it++)
	{
		list<int>::iterator mit = it;
		mit++;

		if (change)
		{
			writeOne.push_back(*it);
		}
		if (!change)
		{
			writeTwo.push_back(*it);
			finished = false;
		}

		if (mit != read.end())
		{
			if (*it > *mit)
			{
				change = !(change);
			}
		}
	}

	if (finished)
	{
		done = true;
	}
}

/*************************************************************************
* join
* Similar to split, join compares values within the runs and pushes them
* onto the first list in sorted order.
**************************************************************************/
void join(list<int> &write, list<int> &readOne, list<int> &readTwo)
{
	write.clear();
	list<int>::iterator it = readOne.begin();
	list<int>::iterator mit = readTwo.begin();
	bool change = false;

	// Although there may be a better way, my while loops did
	// not work the way I wanted them to so I had to add
	// a few extra if statements, just to prevent an
	// infinite loop.
	while ((it != readOne.end()) && (mit != readTwo.end()))
	{
		while ((change == false) && (mit != readTwo.end()))
		{
			if (*it == *mit)
			{
				write.push_back(*it);
				it++;
			}
			if (it == readOne.end())
			{
				break;
			}
			if (*it <= *mit)
			{
				write.push_back(*it);
				it++;
			}
			if (it == readOne.end())
			{
				break;
			}
			if (*it > *mit)
			{
				change = !(change);
				break;
			}
			if (it == readOne.end())
			{
				break;
			}
		}
		while ((change == true) && (it != readOne.end()))
		{
			if (*mit == *it)
			{
				write.push_back(*mit);
				mit++;
			}
			if (mit == readTwo.end())
			{
				break;
			}
			if (*mit <= *it)
			{
				write.push_back(*mit);
				mit++;
			}
			if (mit == readTwo.end())
			{
				break;
			}
			if (*mit > *it)
			{
				change = !(change);
				break;
			}
			if (mit == readTwo.end())
			{
				break;
			}
		}
	}

	// If there are any leftovers in the list, push the rest onto
	// the list being written.
	if (change == true)
	{
		while (it != readOne.end())
		{
			write.push_back(*it);
			it++;
		}
	}
	if (change == false)
	{
		while (mit != readTwo.end())
		{
			write.push_back(*mit);
			mit++;
		}
	}
}