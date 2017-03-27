/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    This contains the List class and ListIterator class, both of which
 *	are templates.  The list is an implementation of a doubly-linked list.
 * The list is composed of Node class pointers that point to both the 
 * previous Node and next Node.  The head's previous pointer is NULL and
 * the tail's next pointer is NULL.  numItems keeps track of the number
 * of Nodes.
 * Author:
 *    Ryan Walker
 ************************************************************************/
#ifndef LIST_H
#define LIST_H

#include <iostream>
#include "node.h"
using namespace std;

// forward declaration for ListIterator
template <class T>
class ListIterator;


template <class T>
class List
{
public:
	int numItems;		// number of Nodes in the List
	Node<T> * head;	// front of the doubley linked-list
	Node<T> * tail;	// tail of the doubley linked-list

	// Default Constructor
	List() : numItems(0), head(NULL), tail(NULL) {}
  
	// Copy Constructor
	List(const List & rhs)
	{
	  	try
	  	{
	  		// uses assignment operator to copy
	  		*this = rhs;
	  	}
	  	catch (std::bad_alloc)
   	{
      	throw "ERROR: Unable to allocate buffer";
   	}
	}
  	
  	// is this List empty?
	bool empty() const 			{ return numItems == 0; }

	// clears all List data and sets head and tail to NULL
	void clear()					{ numItems = 0; freeData(head); tail = NULL; }
  	
  	// returns the number of Nodes in the List as an int
	int size()						{ return numItems;		}
  
  	// add a new Node with the data passed in onto the end of the List
	void push_back(const T & data)  throw (const char *);

	// add a new Node with the data passed in onto the front of the List
	void push_front(const T & data) throw (const char *);

	// returns the front Node data
	T & front() throw (const char *);

	// returns the back Node data
	T & back()  throw (const char *);

	// adds a new Node and data into the List
	void insert(ListIterator<T> it, const T & data);

	// deletes a Node from the List
	void remove(ListIterator<T> it);
  
  	// assignment operator
	List <T> & operator = (const List & rhs);
  
  	// iterates from the head of the List till it hits NULL
	ListIterator <T> begin() { return ListIterator <T>(head); }
  
  	// iterates from the tail of the List till it hits NULL
  	ListIterator <T> rbegin() { return ListIterator <T>(tail); }
  	
  	// end of the List at the tail
  	ListIterator <T> end() { return ListIterator <T>(NULL); }
  
  	// end of the List at the head
  	ListIterator <T> rend() { return ListIterator <T>(NULL); }
};

/************************************
* List <T> :: push_back
* Takes the data to be inserted as a
* parameter and inserts a new node
* with the data at the end of the List.
************************************/
template <class T>
void List <T> :: push_back(const T & data) throw (const char *)
{
	Node<T>* nNode = new Node<T>(data);
	if (tail == NULL)
	{
		// initialize
		head = tail = nNode;
		numItems = 1;
	}
	else
	{
		try
		{
			// insert at end
			tail->pNext = nNode;
	      nNode->pPrev = tail;
	      tail = nNode;
	      numItems++;
		}
		catch (std::bad_alloc)
	   {
	      throw "ERROR: Unable to allocate buffer";
	   }
	}
}

/************************************
* List <T> :: push_front
* Takes the data to be inserted as a
* parameter and inserts a new node
* with the data at the front of the List.
************************************/
template <class T>
void List <T> :: push_front(const T & data) throw (const char *)
{
	Node<T>* nNode = new Node<T>(data);
	if (head == NULL)
	{
		// initialize
		head = tail = nNode;
		numItems = 1;
	}
	else
	{
		try
		{
			// insert at beginning
			nNode->pNext = head;
      	head->pPrev = nNode;
      	head = nNode;
      	numItems++;
		}
		catch (std::bad_alloc)
	   {
	      throw "ERROR: Unable to allocate buffer";
	   }
	}
}

/************************************
* List <T> :: front
* Returns the address of head's data
* so that it can be changed.
************************************/
template <class T>
T & List <T> :: front() throw (const char *)
{
	try
	{
		return head->data;
	}
	catch (std::bad_alloc)
   {
      throw "ERROR: Unable to access data from an empty list";
   }
}

/************************************
* List <T> :: back
* Returns the address of tail's data
* so that it can be changed.
************************************/
template <class T>
T & List <T> :: back() throw (const char *)
{
	try
	{
		return tail->data;
	}
	catch (std::bad_alloc)
   {
      throw "ERROR: Unable to access data from an empty list";
   }
}

/************************************
* List <T> :: insert
* Takes a ListIterator and the data to
* be inserted as a parameter.  It will
* insert according to where it needs 
* to be placed.
************************************/
template <class T>
void List <T> :: insert(ListIterator <T> it, const T & data)
{
   Node<T> * nNode  =  new Node<T>(data);
   if (head == NULL)			// make a new List
	{
      head = tail = nNode;
   }
   else if (it == end())	// insert at end of list
   {
      tail->pNext = nNode;
      nNode->pPrev = tail;
      tail = nNode;
   }
   else
   {
      nNode->pNext = it.p;	// insert at beginning or middle
      nNode->pPrev = it.p->pPrev;
      it.p->pPrev = nNode;
      if (it.p == head)
         head = nNode;		// insert at beginning 
      else
         nNode->pPrev->pNext = nNode;    // insert in middle
   }
   numItems++;	// we have another Node
}

/************************************
* List <T> :: operator=
* Overrides = to copy a List
************************************/
template <class T>
List <T> & List <T> :: operator = (const List & rhs)
{
	Node<T> *node = rhs.head;
	Node<T> *copy = new Node<T>;
	copy->data = node->data;
	head = copy; // We do not loop through yet because we want to keep track of the head
	node = node->pNext;

	// Now we can loop through
	while(node)
	{
		Node<T>* prev = copy;
		copy->pNext = new Node<T>;
		copy = copy->pNext;
		copy->data = node->data;
		copy->pPrev = prev;
		node = node->pNext;
	}
	tail = copy;
	numItems = rhs.numItems;

	// return *this as the copied List
	return *this;
}

/************************************
* List <T> :: remove
* Takes a ListIterator as a parameter
* pointing to the Node to be removed.
* Removes that Node and changes
* pointers accordingly.
************************************/
template <class T>
void List <T> :: remove(ListIterator<T> it)
{
	if (it == end())
	{
		throw "ERROR: unable to remove from an invalid location in a list";
	}
	else
	{
		if (it.p->pPrev)		// not the head
		{
			it.p->pPrev->pNext = it.p->pNext;
			if (it.p->pNext)	// not the tail
				it.p->pNext->pPrev = it.p->pPrev;
			else					// tail
				tail = tail->pPrev;
		}
		else if (it.p->pPrev == NULL) // head
		{
			it.p->pNext->pPrev = NULL;
			head = head->pNext;
		}
		numItems--;	// we're losing a node.
	}
}

template <class T>
class ListIterator
{
  public:
   // default constructor
   ListIterator() : p(NULL) {}

   // initialize to direct p to some item
   ListIterator(Node<T> * p) : p(p) {}

   // copy constructor
   ListIterator(const ListIterator & rhs) { *this = rhs; }

   friend void List<T> :: insert(ListIterator<T> it, const T & data);
   friend void List<T> :: remove(ListIterator<T> it);

   // assignment operator
   ListIterator & operator = (const ListIterator & rhs)
   {
      this->p = rhs.p;
      return *this;
   }

   // comparison operator
   bool operator == (const ListIterator & rhs) const
   {
      return rhs.p == this->p;
   }

   // not equals operator
   bool operator != (const ListIterator & rhs) const
   {
      return rhs.p != this->p;
   }

   // dereference operator
   T & operator * ()
   {
      return p->data;
   }

   // prefix increment
   ListIterator <T> & operator ++ ()
   {
      p = p->pNext;
      return *this;
   }

   // postfix increment
   ListIterator <T> operator++(int postfix)
   {
      ListIterator tmp(*this);
      p = p->pNext;
      return tmp;
   }
   
   // prefix decrement
   ListIterator <T> & operator --()
   {
      p = p->pPrev;
      return *this;
   }
   
   
  private:
   Node<T> * p;
};


#endif // LIST_H