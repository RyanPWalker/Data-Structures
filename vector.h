/***************************************************************
 * File: vector.h
 * Author: Ryan Walker
 * Purpose: Contains the definition of the Vector class
 ***************************************************************/
#ifndef Vector_H
#define Vector_H

#include <cassert>
#include <iostream>

using namespace std;

// forward declaration for VectorIterator
template <class T>
class VectorIterator;

/************************************************
 * Vector
 * A class that holds stuff
 ***********************************************/
template <class T>
class Vector
{
public:

   T * data;          // dynamically allocated array of T
   int numItems;      // how many items are currently in the Vector?
   int cap;           // how many items can I put on the Vector before full?

   // default constructor : empty and kinda useless
   Vector() : numItems(0), cap(0), data(NULL) {}

   // copy constructor : copy it
   Vector(const Vector & rhs) throw (const char *);
   
   // non-default constructor : pre-allocate
   Vector(int cap) throw (const char *);
   
   // destructor : free everything
   ~Vector()           { if (cap) delete [] data;      }

   // overloading operators
   Vector<T>  & operator=(Vector<T> rhs);
   const T & operator[](int num) const {return data[num];}
   T & operator[](int num) {return data[num];}
   
   // is the vector empty?  (numItems == 0?)
   bool empty() const   { return numItems == 0;         }

   // clear the contents (NOT THE CAPACITY!)
   void clear()         { numItems = 0;                 }

   // add a variable to the array
   void push_back(const T & add)  throw (const char *);

   // number of items in the array
   int size() const     { return numItems;              }

   // total number of spaces available in the array
   int capacity() const { return cap;                   }

   // add an item to the Vector
   void insert(const T & t) throw (const char *);
   
   // return an iterator to the beginning of the list
   VectorIterator <T> begin() { return VectorIterator<T>(data); }

   // return an iterator to the end of the list
   VectorIterator <T> end() { return VectorIterator<T>(data + numItems);}
   
};

/**************************************************
 * Vector ITERATOR
 * An iterator through Vector
 *************************************************/
template <class T>
class VectorIterator
{
  public:
   // default constructor
  VectorIterator() : p(NULL) {}

   // initialize to direct p to some item
  VectorIterator(T * p) : p(p) {}

   // copy constructor
   VectorIterator(const VectorIterator & rhs) { *this = rhs; }

   // assignment operator
   VectorIterator & operator = (const VectorIterator & rhs)
   {
      this->p = rhs.p;
      return *this;
   }

   // not equals operator
   bool operator != (const VectorIterator & rhs) const
   {
      return rhs.p != this->p;
   }

   // dereference operator
   T & operator * ()
   {
      return *p;
   }

   // prefix increment
   VectorIterator <T> & operator ++ ()
   {
      p++;
      return *this;
   }

   // postfix increment
   VectorIterator <T> operator++(int postfix)
   {
      VectorIterator tmp(*this);
      p++;
      return tmp;
   }

   // prefix decrement
   VectorIterator <T> & operator -- ()
   {
      p--;
      return *this;
   }

   // postfix decrement
   VectorIterator <T> operator--(int postfix)
   {
      VectorIterator tmp(*this);
      p--;
      return tmp;
   }
   
  private:
   T * p;
};

/*******************************************
 * Vector :: COPY CONSTRUCTOR
 *******************************************/
template <class T>
Vector <T> :: Vector(const Vector <T> & rhs) throw (const char *)
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
 * Vector : NON-DEFAULT CONSTRUCTOR
 * Preallocate the Vector to "cap"
 **********************************************/
template <class T>
Vector <T> :: Vector(int cap) throw (const char *)
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

   // initialize the Vector by calling the default constructor
   for (int i = 0; i < cap; i++)
      data[i] = T();
}

/************************************
* Vector :: OPERATOR=
* Overrides = to copy any value.
************************************/
template <class T>
Vector<T> & Vector <T> :: operator=(Vector<T> rhs)
{
	delete [] data;
	cap = rhs.cap;
	numItems = rhs.numItems;
	data = new T[cap];

   for (int i = 0; i < numItems; i++)
   {
   	data[i] = rhs.data[i];
   }
      
   return *this;
}

/*****************************************
* Vector :: PUSH_BACK
* Adds an object onto the vector
*****************************************/
template <class T>
void Vector <T> :: push_back(const T & add)  throw (const char *)
{
   if (cap == 0)
   {
      cap = 1;			 //initalize capacity to create space in the vector
      data = new T[cap]; //initialize the data so there's no segmentation fault
   }

   try
   {  
      if (numItems >= cap)
      {
         T *nData = new T[cap *= 2];

         for (int i = 0; i < numItems; i++)
         {
            nData[i] = data[i];
         }

         delete [] data;
         data = nData;
      }
      
      data[numItems++] = add;
   }
   catch (std::bad_alloc)
   {
      throw "ERROR: Unable to allocate a new buffer for vector";
   }
}

/***************************************************
 * Vector :: INSERT
 * Insert an item on the end of the Vector
 **************************************************/
template <class T>
void Vector <T> :: insert(const T & t) throw (const char *)
{
   // do we have space?
   if (cap == 0 || cap == numItems)
      throw "ERROR: Insufficient space";
   
   // add an item to the end
   data[numItems++] = t;

   //cout << "insert  " << endl;
   //cout << *data << endl;
   //cout << numItems << endl;
   //cout << cap << endl;
}


#endif // Vector_H
