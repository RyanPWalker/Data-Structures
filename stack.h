/***************************************************************
 * File: Stack.h
 * Author: Ryan Walker
 * Purpose: Contains the definition of the Stack class
 ***************************************************************/
#ifndef STACK_H
#define STACK_H

#include <cassert>
#include <iostream>

using namespace std;

/************************************************
 * Stack
 * A class that holds data.  Last in, first out.
 ***********************************************/
template <class T>
class Stack
{
public:
   T * data;          // dynamically allocated array of T
   int numItems;      // how many items are currently in the Stack?
   int cap;           // how many items can I put on the Stack before full?

   // default constructor : empty and kinda useless
   Stack() : numItems(0), cap(0), data(NULL) {}

   // copy constructor : copy it
   Stack(const Stack & rhs) throw (const char *);
   
   // non-default constructor : pre-allocate
   Stack(int cap) throw (const char *);
   
   // destructor : free everything
   ~Stack()             { if (cap) delete [] data;      }

   //Assignment operator
   Stack <T> & operator=(Stack <T> & rhs);

   // Is the stack empty?
   bool empty() const   { return numItems == 0; }

   // Number of items within stack
   int size()   const   { return numItems;      }

   // Space available within stack
   int capacity() const { return cap;           }

   // Clears the stack of items, not capacity
   void clear()         { numItems = 0;         }

   // Adds an item to the top of the stack (Last in, First out)
   void push(const T & add)  throw (const char *);

   // Removes the top item from the stack
   void pop()   throw (const char *);

   // Returns the top item on the stack
   T & top()    throw (const char *);
};

/*******************************************
 * Stack :: COPY CONSTRUCTOR
 *******************************************/
template <class T>
Stack <T> :: Stack(const Stack <T> & rhs) throw (const char *)
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
   for (int i = 0; i < numItems; i++)
      data[i] = rhs.data[i];

   // the rest needs to be filled with the default value for T
   for (int i = numItems; i < cap; i++)
      data[i] = T();
}

/**********************************************
 * Stack : NON-DEFAULT CONSTRUCTOR
 * Preallocate the Stack to "cap"
 **********************************************/
template <class T>
Stack <T> :: Stack(int cap) throw (const char *)
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

      
   // copy over the stuff
   this->cap = cap;
   this->numItems = 0;

   // initialize the Stack by calling the default constructor
   for (int i = 0; i < cap; i++)
      data[i] = T();
}

/************************************
* Stack :: operator=
* Overrides = to copy any value.
************************************/
template <class T>
Stack <T> & Stack <T> :: operator=(Stack <T> & rhs)
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

/*******************************************
 * Stack :: pop
 * Removes the top item off the stack.
 *******************************************/
template <class T>
void Stack <T> :: pop()   throw (const char *)
{
   if (!size())
      throw "ERROR: Unable to pop from an empty Stack";
   else
      numItems--;
}

/*******************************************
 * Stack :: top
 * Returns the top item on the stack.
 *******************************************/
template <class T>
T & Stack <T> :: top()   throw (const char *)
{
   if (numItems <= 0)
      throw "ERROR: Unable to reference the element from an empty Stack";
   else
      return data[numItems - 1];
}

/*****************************************
* Stack::push
* Adds an object onto the Stack
*****************************************/
template <class T>
void Stack <T> :: push(const T & add)  throw (const char *)
{
   if (cap == 0)
   {
      cap = 1;           //initalize capacity to create space in the stack
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
      
      // Now we can add the new item
      data[numItems++] = add;
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate a new buffer for vector";
   }
}

#endif // Stack_H