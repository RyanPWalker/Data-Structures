/***************************************************************
 * File: Set.h
 * Author: Ryan Walker
 * Purpose: Contains the definition of the Set class.
 ***************************************************************/
#ifndef Set_H
#define Set_H

#include <cassert>
#include <iostream>

using namespace std; // for debugging

// forward declaration for SetIterator
template <class T>
class SetIterator;

/************************************************
 * Set
 * A data structure that holds and orders by value
 * any data type.  Doubles in size when it's
 * capacity is reached.
 ***********************************************/
template <class T>
class Set
{
public:

   T * data;          // dynamically allocated array of T
   int numItems;      // how many items are currently in the Set?
   int cap;           // how many items can I put on the Set before full?

   // Default constructor : empty and kinda useless
   Set() : numItems(0), cap(0), data(NULL) {}

   // Copy constructor : copy it
   Set(const Set & rhs) throw (const char *);
   
   // Non-default constructor : pre-allocate
   Set(int cap) throw (const char *);
   
   // Destructor : free everything
   ~Set()             { if (cap) delete [] data;    }

   // Assignment operator
   Set <T> & operator=(Set <T> & rhs);

   // Intersecting operator
   Set <T> operator&&(Set <T> & rhs);

   // Union operator
   Set <T> operator||(Set <T> & rhs);

   // Is the Set empty?
   bool empty() const   { return numItems == 0; }

   // Number of items within Set
   int size()   const   { return numItems;      }

   // Space available within Set
   int capacity() const { return cap;           }

   // Clears the Set of items, not cap
   void clear()         { numItems = 0; 	    }

   // Add an item to the Set
   void insert(const T & t) throw (const char *);

   // Remove an item from the Set
   void erase(int remove);

   // Search for an item in the Set
   int find(const T & get);
   
   // return an iterator to the beginning of the list
   SetIterator <T> begin() { return SetIterator<T>(data); }

   // return an iterator to the end of the list
   SetIterator <T> end() { return SetIterator<T>(data + numItems);}
};


/**************************************************
 * Set ITERATOR
 * An iterator through Set
 *************************************************/
template <class T>
class SetIterator
{
  public:
   // default constructor
  SetIterator() : p(NULL) {}

   // initialize to direct p to some item
  SetIterator(T * p) : p(p) {}

   // copy constructor
   SetIterator(const SetIterator & rhs) { *this = rhs; }

   // assignment operator
   SetIterator & operator = (const SetIterator & rhs)
   {
      this->p = rhs.p;
      return *this;
   }

   // not equals operator
   bool operator != (const SetIterator & rhs) const
   {
      return rhs.p != this->p;
   }

   // dereference operator
   T & operator * ()
   {
      return *p;
   }

   // prefix increment
   SetIterator <T> & operator ++ ()
   {
      p++;
      return *this;
   }

   // postfix increment
   SetIterator <T> operator++(int postfix)
   {
      SetIterator tmp(*this);
      p++;
      return tmp;
   }
   
  private:
   T * p;
};

/*******************************************
 * Set :: COPY CONSTRUCTOR
 *******************************************/
template <class T>
Set <T> :: Set(const Set <T> & rhs) throw (const char *)
{
   assert(rhs.cap >= 0);

   // do nothing if there is nothing to do
   if (rhs.cap == 0)
   {
      cap = numItems = 0;
      data = NULL;
      return;
   }

   // attempt to allocate
   try
   {
      data = new T[rhs.cap];
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate buffer";
   }
   
   // copy over the cap and size
   assert(rhs.numItems >= 0 && rhs.numItems <= rhs.cap);
   cap = rhs.cap;
   numItems = rhs.numItems;

   // copy the items over one at a time using the assignment operator
   for (int i = 0; i < cap; i++)
      data[i] = rhs.data[i];
}

/**********************************************
 * Set : NON-DEFAULT CONSTRUCTOR
 * Preallocate the Set to "cap"
 **********************************************/
template <class T>
Set <T> :: Set(int cap) throw (const char *)
{
   assert(cap >= 0);
   
   // do nothing if there is nothing to do
   if (cap == 0)
   {
      this->cap = this->numItems = 0;
      this->data = NULL;
      return;
   }

   // attempt to allocate
   try
   {
      data = new T[cap];
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate buffer";
   }

      
   // copy over the cap and size
   this->cap = cap;
   this->numItems = 0;

   // initialize the Set by calling the default constructor
   for (int i = 0; i < cap; i++)
      data[i] = T();
}

/************************************
* Set :: operator=
* Overrides = to copy any value.
************************************/
template <class T>
Set <T> & Set <T> :: operator=(Set <T> & rhs)
{
   // stop those memory leaks
   delete [] data;

   //copy over the cap and size
   cap = rhs.cap;
   numItems = rhs.numItems;
   data = new T[cap];

   // copy the data
   for (int i = 0; i < numItems; i++)
   {
      data[i] = rhs.data[i];
   }

   return *this;
}

/************************************
* Set :: operator&&
* Intersects two sets.
* Matching values stay.
************************************/
template <class T>
Set <T> Set <T> :: operator&&(Set <T> & rhs)
{
	Set <T> intersect;
	for (int i = 0; i < numItems; i++)
	{
      if (rhs.find(data[i]) != -1)  // Can we find a match in the rhs?
         intersect.insert(data[i]); // We have a match!
	}

   return intersect;
}

/************************************
* Set :: operator||
* Unifies two sets.  Everything
* except duplicate values stays.
************************************/
template <class T>
Set <T> Set <T> :: operator||(Set <T> & rhs)
{
	Set <T> unionSet;
   int limit = 0;

   while (data[limit]) // everything in this set
   {
      unionSet.insert(data[limit]);
      limit++;
   }

   limit = 0;
   while (rhs.data[limit]) // everything in this set
   {

   	// insert() will automatically take duplicates out
      unionSet.insert(rhs.data[limit]);
      limit++;
   }

   return unionSet;
}

/***************************************************
 * Set :: insert
 * Takes a template as a paramter and adds it
 * into the set by order value.
 **************************************************/
template <class T>
void Set <T> :: insert(const T & t) throw (const char *)
{
   if (cap == 0)
   {
      cap = 2;           //initalize capacity to create space in the stack
      data = new T[cap]; //initialize the data so there's no segmentation fault
   }

   try
   {  
      if (numItems >= cap)
      {
         // Double capacity in the new array
         T *nData = new T[cap *= 2];

         for (int i = 0; i < numItems; i++)
         {
         	// Copy the data
            nData[i] = data[i];
         }

         delete [] data;
         // Copy the pointer
         // This is preferred opposed to copying the data into another array
         data = nData;
      }

      int add = find(t); // Is it already in the Set?
      if (add == -1)
      {
      	int place = numItems;  // Initially could add item to the end
      	for (int i = 0; i < numItems; i++)
	      {
	      	if (data[i] > t)
	      	{
	      		place = i;
	      		break;
	      	}
	      }
	      for (int i = numItems - 1; i >= place; i--)
	      {
	      	data[i + 1] = data[i]; // shift data to the right
	      }
	      // Now we can insert the new item
	      data[place] = t;
	      numItems++;
	   }
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate a new buffer for vector";
   }
}

/***************************************************
 * Set :: erase
 * Takes an integer parameter as an index for the
 * item to be removed.
 **************************************************/
template <class T>
void Set <T> :: erase(int remove)
{
	numItems--;
	for (int i = remove; i < numItems; i++)
	{
		// remove item and shift to the left
		data[i] = data[i + 1];
	}

	// gotta get the last one
	if (remove == numItems)
	{
		data[remove] = "\0";
	}

}

/***************************************************
 * Set :: find
 * Takes a template parameter returns an item in
 * the Set.  If the item is not found, it returns -1.
 **************************************************/
template <class T>
int Set <T> :: find(const T & get)
{
	for (int i = 0; i < cap; i++)
	{
		if (data[i] == get)
			// found it
			return i;
	}

	return -1;
}

#endif // Set_H