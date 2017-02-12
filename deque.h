/***************************************************************
 * File: Deque.h
 * Author: Ryan Walker
 * Purpose: Contains the definition of the Deque class.
 ***************************************************************/
#ifndef Deque_H
#define Deque_H

#include <cassert>
#include <iostream>

using namespace std;


/************************************************
 * Deque
 * A double-ended queue that holds any data type
 * and doubles in size when it's capacity is reached.
 ***********************************************/
template <class T>
class Deque
{
public:

   T * data;          // dynamically allocated array of T
   int numItems;      // how many items are currently in the Deque?
   int cap;           // how many items can I put on the Deque before full?
   int myFront;       // front item on the Deque
   int myBack;        // back item on the Deque

   // Default constructor : empty and kinda useless
   Deque() : myFront(0), myBack(0), numItems(0), cap(0), data(NULL) {}

   // Copy constructor : copy it
   Deque(const Deque & rhs) throw (const char *);
   
   // Non-default constructor : pre-allocate
   Deque(int cap) throw (const char *);
   
   // Destructor : free everything
   ~Deque()             { if (cap) delete [] data;    }

   // Assignment operator
   Deque <T> & operator=(Deque <T> & rhs);

   // Is the Deque empty?
   bool empty() const   { return numItems == 0;       }

   // Number of items within Deque
   int size()   const   { return numItems;            }

   // Space available within Deque
   int capacity() const { return cap;                 }

   // Clears the Deque of items, not capacity
   void clear()         { numItems = 0; myFront = 0; myBack = 0; }

   // Reallocates more space
   void realloc();

   // Adds an item to the front of the Deque
   void push_front(const T & add)  throw (const char *);

   // Adds an item to the back of the Deque
   void push_back(const T & add)  throw (const char *);

   // Removes the front item from the Deque
   void pop_front()   throw (const char *);

   // Removes the back item from the Deque
   void pop_back()   throw (const char *);

   // Returns the item at the front of the Deque
   T & front()   throw (const char *);

   // Returns the item at the back of the Deques
   T & back()  throw (const char *);
};


/*******************************************
 * Deque :: COPY CONSTRUCTOR
 *******************************************/
template <class T>
Deque <T> :: Deque(const Deque <T> & rhs) throw (const char *)
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
 * Deque : NON-DEFAULT CONSTRUCTOR
 * Preallocate the Deque to "cap"
 **********************************************/
template <class T>
Deque <T> :: Deque(int cap) throw (const char *)
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

      
   // copy over the cap, size, front, and back
   this->cap = cap;
   this->numItems = 0;
   myFront = 0;
   myBack = 0;

   // initialize the Deque by calling the default constructor
   for (int i = 0; i < cap; i++)
      data[i] = T();
}

/************************************
* Deque :: operator=
* Overrides = to copy any value.
************************************/
template <class T>
Deque <T> & Deque <T> :: operator=(Deque <T> & rhs)
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
 * Deque :: pop_front
 * Removes the front item off the Deque.
 *******************************************/
template <class T>
void Deque <T> :: pop_front()   throw (const char *)
{
   if (!size())
   {
      throw "ERROR: unable to pop from the front of empty deque";
   }

   // move the front
   myFront = (myFront + 1) % cap;
   numItems--;
}

/*******************************************
 * Deque :: pop_back
 * Removes the front item off the Deque.
 *******************************************/
template <class T>
void Deque <T> :: pop_back()   throw (const char *)
{
   if (!size())
   {
      throw "ERROR: unable to pop from the back of empty deque";
   }

   // move the back
   myBack--;
   if (myBack < 0)
   {
      myBack = cap - 1;
   }

   numItems--;
}

/******************************************
 * Deque :: front
 * Returns the front item on the Deque
 *******************************************/
template <class T>
T & Deque <T> :: front()   throw (const char *)
{
      // make sure the deque isn't empty or front isn't less than zero
   if (numItems == 0)
      throw "ERROR: unable to access data from an empty deque";
   else
           return data[myFront];
}

/*******************************************
 * Deque :: back
 * Returns the back item on the Deque
 *******************************************/
template <class T>
T & Deque <T> :: back()   throw (const char *)
{
   // make sure the deque isn't empty or back isn't less than zero
   if (numItems == 0)
      throw "ERROR: unable to access data from an empty deque";
   else if (myBack == 0)
      return data[(myBack - 1 + cap) % cap];
   else
      return data[myBack - 1];
}

/*******************************************
 * Deque :: realloc
 * Called by the push_back and push_front
 * functions.  If the deque is full it will
 * copy the data into a new deque twice the
 * size.
 *******************************************/
template <class T>
void Deque <T> :: realloc()
{
   int oldCap = cap;
   cap *= 2;
   T *nData = new T[cap];

   for (int i = 0; i < numItems; i++)
   {
      // copy the data
      nData[i] = data[((myFront + i) % oldCap)]; // circular Deque.  Copy them in order
   }

   delete [] data;
   data = nData;

   // reset front and back end
   myFront = 0;
   if (size())
      myBack = numItems;
}

/*****************************************
* Deque :: push_front
* Adds an object onto front of the Deque
*****************************************/
template <class T>
void Deque <T> :: push_front(const T & add)  throw (const char *)
{
   try
   {
      // if the deque is full, reallocate to make space
      if (cap == 0)
      {
         cap = 2;
         data = new T[cap];
         realloc();
      }
      else if (numItems >= cap)
      {
         realloc();
      }
      else if (((myBack % cap) == myFront) && (numItems == cap))
      {
         realloc();
      }

      // move the front
      myFront--;
      if (myFront < 0)
         myFront = cap - 1;

      // now we can add the new item
      data[myFront] = add;
      numItems++;

   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate a new buffer for Deque";
   }
}

/*****************************************
* Deque :: push_back
* Adds an object onto the back of the Deque
*****************************************/
template <class T>
void Deque <T> :: push_back(const T & add)  throw (const char *)
{
   try
   {
      // if the deque is full, reallocate to make space
      if (cap == 0)
      {
         cap = 2;
         data = new T[cap];
         realloc();
      }
      else if (numItems >= cap)
      {
         realloc();
      }
      else if (((myBack % cap) == myFront) && (numItems == cap))
      {
         realloc();
         cerr << "third realloc" << endl;
      }

      // now we can add the new item

      data[myBack] = add;
      myBack = (myBack + 1) % cap; // move the back
      numItems++;
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate a new buffer for Deque";
   }
}


#endif // Deque_H