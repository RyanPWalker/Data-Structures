/***************************************************************
 * File: Queue.h
 * Author: Ryan Walker
 * Purpose: Contains the definition of the Queue class.
 ***************************************************************/
#ifndef Queue_H
#define Queue_H

#include <cassert>
#include <iostream>

using namespace std;


/************************************************
 * Queue
 * A circular queue that holds stuff and doubles
 * in size when it's capacity is reached.
 * First in, first out.
 ***********************************************/
template <class T>
class Queue
{
public:

   T * data;          // dynamically allocated array of T
   int numItems;      // how many items are currently in the Queue?
   int cap;           // how many items can I put on the Queue before full?
   int myFront;       // front item on the queue
   int myBack;        // back item on the queue

   // default constructor : empty and kinda useless
   Queue() : myFront(0), myBack(0), numItems(0), cap(0), data(NULL) {}

   // copy constructor : copy it
   Queue(const Queue & rhs) throw (const char *);
   
   // non-default constructor : pre-allocate
   Queue(int cap) throw (const char *);
   
   // destructor : free everything
   ~Queue()             { if (cap) delete [] data; }

   //Assignment operator
   Queue <T> & operator=(Queue <T> & rhs);

   // Is the Queue empty?
   bool empty() const   { return numItems == 0;    }

   // Number of items within Queue
   int size()   const   { return numItems;         }

   // Space available within Queue
   int capacity() const { return cap;              }

   // Clears the Queue of items, not capacity
   void clear()         { numItems = 0; myFront = 0; myBack = 0; }

   // Reallocates more space
   void realloc();

   // Adds an item to the top of the Queue (Last in, First out)
   void push(const T & add)  throw (const char *);

   // Removes the top item from the Queue
   void pop()   throw (const char *);

   // Returns the item at the front of the Queue
   T & front()   throw (const char *);

   // Returns the item at the back of the Queues
   T & back()  throw (const char *);
};


/*******************************************
 * Queue :: COPY CONSTRUCTOR
 *******************************************/
template <class T>
Queue <T> :: Queue(const Queue <T> & rhs) throw (const char *)
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
   
   // copy over the cap, size, front, and back
   assert(rhs.numItems >= 0 && rhs.numItems <= rhs.cap);
   cap = rhs.cap;
   numItems = rhs.numItems;
   myFront = rhs.myFront;
   myBack = rhs.myBack;

   // copy the items over one at a time using the assignment operator
   for (int i = 0; i < cap; i++)
      data[i] = rhs.data[i];
}

/**********************************************
 * Queue : NON-DEFAULT CONSTRUCTOR
 * Preallocate the Queue to "cap"
 **********************************************/
template <class T>
Queue <T> :: Queue(int cap) throw (const char *)
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
   myFront = 0;
   myBack = 0;

   // initialize the Queue by calling the default constructor
   for (int i = 0; i < cap; i++)
      data[i] = T();
}

/************************************
* Queue :: operator=
* Overrides = to copy any value.
************************************/
template <class T>
Queue <T> & Queue <T> :: operator=(Queue <T> & rhs)
{
   // stop those memory leaks
   delete [] data;

   //copy over the cap, size, front, and back
   cap = rhs.cap;
   numItems = rhs.numItems;
   data = new T[cap];
   myFront = rhs.myFront;
   myBack = rhs.myBack;

   // copy the data
   for (int i = 0; i < numItems; i++)
   {
      data[i] = rhs.data[i];
   }

   return *this;
}

/*******************************************
 * Queue :: pop
 * Removes the front item off the Queue.
 *******************************************/
template <class T>
void Queue <T> :: pop()   throw (const char *)
{
   if (!size())
   {
      throw "ERROR: attempting to pop from an empty queue";
   }

   // move the front
   myFront = (myFront + 1) % cap;
   numItems--;
}

/******************************************
 * Queue :: front
 * Returns the front item on the Queue
 *******************************************/
template <class T>
T & Queue <T> :: front()   throw (const char *)
{
   if (numItems == 0)
   {
      throw "ERROR: attempting to access an item in an empty queue";
   }

    return data[myFront];
}

/*******************************************
 * Queue :: back
 * Returns the back item on the Queue
 *******************************************/
template <class T>
T & Queue <T> :: back()   throw (const char *)
{
      if (numItems == 0)
   {
      throw "ERROR: attempting to access an item in an empty queue";
   }
   
    return data[myBack - 1];
}

template <class T>
void Queue <T> :: realloc()
{
   T *nData = new T[cap];

   for (int i = 0; i < numItems; i++)
   {
      // Copy the data
      nData[i] = data[((myFront + i) % (cap / 2))]; // Circular queue.  Copy them in order
   }

   delete [] data;
   // Copy the pointer
   // This is preferred opposed to copying the data into another array
   data = nData;

   // Reset front and back end
   myFront = 0;
   myBack = numItems;
}

/*****************************************
* Queue::push
* Adds an object onto the Queue
*****************************************/
template <class T>
void Queue <T> :: push(const T & add)  throw (const char *)
{

   try
   {
      if (cap == 0)
      {
         cap = 2;
         realloc();
      }
      else if (numItems >= cap)
      {
         cap *= 2;
         realloc();
      }
      else if (((myBack % cap) == myFront) && (numItems == cap))
      {
         cap *= 2;
         realloc();
      }

      numItems++;
      // Now we can add the new item
      data[myBack] = add;
      myBack = (myBack + 1) % cap;
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate a new buffer for queue";
   }
}


#endif // Queue_H