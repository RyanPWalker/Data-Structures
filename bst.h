/***********************************************************************
 * Component:
 *    Week 09, Binary Search Tree (BST)
 *    Brother Ercanbrack, CS 235
 * Author:
 *    Ryan Walker
 * Summary:
 *    Data-structure implementation of a binary search tree.
 * Contains a BinaryNode "root" that serves as the beginning of
 * the tree.  The BST automatically sorts what is inserted and
 * can find any value within.
 ************************************************************************/

#ifndef BST_H
#define BST_H

#include "bnode.h"    // for BinaryNode
#include "stack.h"    // for Stack
#include <iostream>

using namespace std;

// forward declaration for the BST iterator
template <class T>
class BSTIterator; 

/*****************************************************************
 * BINARY SEARCH TREE
 * Similar to a binary tree, but is searchable and
 * sorts its data by value.
 *****************************************************************/
template <class T>
class BST
{
private:

   BinaryNode <T> * root;  // beginning of the tree

public:
   // constructor
   BST(): root(NULL){};
   
   // copy constructor
   BST(const BST & rhs);    
   
   // destructor
   ~BST();

   // how many nodes
   int  size()  const { return empty() ? 0 : root->size();   }
   
   // determine if the tree is empty
   bool empty() const { return root ? false : true;          }

   // clear all the contests of the tree
   void clear()       { root = NULL; deleteBinaryTree(root); }

   // overloaded assignment operator
   BST & operator= (const BST & rhs)
   {
      BinaryNode<T>* tmp = rhs.root;
      std::swap(this->root, tmp);
      return *this;
   }
      
   // insert an item
   void insert(const T & t) throw (const char * );

   // remove an item
   void remove(BSTIterator <T> & it);

   // find a given item
   BSTIterator <T> find(const T & t);

   // the usual iterator stuff
   BSTIterator <T> begin() const;
   BSTIterator <T> end() const  { return BSTIterator <T> (NULL) ; }
   BSTIterator <T> rbegin() const;
   BSTIterator <T> rend() const  { return BSTIterator <T> (NULL); }
};

/*********************************************************
* copy constructor
**********************************************************/
template <class T>
BST<T>::BST(const BST &rhs)
{
   *this = rhs;
}

/*****************************************************
* Destructor
*******************************************************/
template <class T>
BST<T>::~BST()
{
   deleteBinaryTree(root);
   root = NULL;
}


/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <class T>
BSTIterator <T> BST <T> :: begin() const
{
   Stack < BinaryNode <T> * > nodes;

   nodes.push(NULL);
   nodes.push(root);
   while (nodes.top() != NULL && nodes.top()->pLeft)
      nodes.push(nodes.top()->pLeft);

   return BSTIterator<T>(nodes);   
}

/*****************************************************
 * BST :: RBEGIN
 * Return the last node (right-most) in a binary search tree
 ****************************************************/
template <class T>
BSTIterator <T> BST <T> :: rbegin() const
{
   Stack < BinaryNode <T> * > nodes;

   nodes.push(NULL);
   nodes.push(root);
   while (nodes.top() != NULL && nodes.top()->pRight)
      nodes.push(nodes.top()->pRight);

   return BSTIterator<T>(nodes);
}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <class T>
void BST <T> :: insert(const T & t) throw (const char *)
{
	BinaryNode <T> * ptr = root;
	BinaryNode <T> * parent = root;

   try
   {
   	while (ptr != NULL) // find the spot to insert
   	{
   		parent = ptr;
   		if (t <= ptr->data)
   		{
   			ptr = ptr->pLeft;
   		}
   		else if (t > ptr->data)
   		{
   			ptr = ptr->pRight;
   		}
   	}

   	ptr = new BinaryNode<T>(t);
   	if (parent == NULL)
   	{
   		root = ptr;
   	}
   	else if (t <= parent->data)
   	{
   		parent->pLeft = ptr;
         ptr->pParent = parent;
   	}
   	else
   	{
   		parent->pRight = ptr;
         ptr->pParent = parent;
   	}
   }
   catch(std::bad_alloc)
   {
      throw "ERROR: Unable to allocate a node";
   }
}

/*************************************************
 * BST :: REMOVE
 * Remove a given node as specified by the iterator
 ************************************************/
template <class T>
void BST <T> :: remove(BSTIterator <T> & it)
{
	BinaryNode <T> * node = it.getNode();

   BSTIterator <T> found;
   found = find(node->data);
   if (found == end())
      return; // it's not found so it can't be removed

	if (node->pLeft && node->pRight) // has two children to reroute
	{
		BinaryNode <T> * ptr = node->pRight;

		if (ptr->pLeft)
		{
			while (ptr->pLeft != NULL)
			{
				ptr = ptr->pLeft;
			}

			if (ptr->pRight)
			{
				ptr->pRight->pParent = ptr->pParent;
				ptr->pParent->pLeft = ptr->pRight;
			}
			node->data = ptr->data;
		}
		else
		{
			node->data = ptr->data;
			node->pRight = ptr->pRight;
			ptr->pRight->pParent = node;
		}

		delete ptr;
	}
	else if (node->pLeft && node->pRight == NULL) // has a left child to reroute
	{
		node->pLeft->pParent = node->pParent;
		if (node->pParent->pLeft == node)
		{
			node->pParent->pLeft = node->pLeft;
		}
		else if (node->pParent->pRight == node)
		{
			node->pParent->pRight = node->pLeft;
		}

		delete node;
	}
	else if (node->pRight && node->pLeft == NULL) // has a right child to reroute
	{
		node->pRight->pParent = node->pParent;
		if (node->pParent->pLeft == node)
		{
			node->pParent->pLeft = node->pRight;
		}
		else if (node->pParent->pRight == node)
		{
			node->pParent->pRight = node->pRight;
		}
		
		delete node;
	}
	else if (node->pRight == NULL && node->pLeft == NULL) // doesn't have any children to reroute
	{
		if (node->pParent->pLeft == node)
		{
			node->pParent->pLeft = NULL;
		}
		else if (node->pParent->pRight == node)
		{
			node->pParent->pRight = NULL;
		}
		
		delete node;
	}
}

/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <class T>
BSTIterator <T> BST <T> :: find(const T & t)
{
	BinaryNode <T> * ptr = root;
	bool found = false;

   while(!found && ptr != NULL)
   {
   	if (t < ptr->data)
   	{
   		ptr = ptr->pLeft;
   	}
   	else if (t > ptr->data)
   	{
   		ptr = ptr->pRight;
   	}
   	else // it's equal to or is not found within the tree
   	{
   		found = true;
   	}
   }
   
	Stack <BinaryNode <T> *> stack;
	stack.push(ptr);

	return BSTIterator<T>(stack);
}

/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <class T>
class BSTIterator
{
public:
   // constructors
   BSTIterator(BinaryNode <T> * p = NULL)    { nodes.push(p);     }
   BSTIterator(Stack <BinaryNode <T> *> & s) { nodes = s;         }
   BSTIterator(const BSTIterator <T> & rhs)  { nodes = rhs.nodes; }

   // assignment
   BSTIterator <T> & operator = (const BSTIterator <T> & rhs)
   {
      // need an assignment operator for the Stack class.
      nodes = rhs.nodes;
      return *this;
   }

   // compare
   bool operator == (const BSTIterator <T> & rhs) const
   {
      // only need to compare the leaf node 
      return rhs.nodes.const_top() == nodes.const_top();
   }
   bool operator != (const BSTIterator <T> & rhs) const
   {
      // only need to compare the leaf node 
      return rhs.nodes.const_top() != nodes.const_top();
   }

   // de-reference. Cannot change because it will invalidate the BST
   T & operator * ()  
   {
      return nodes.top()->data;
   }

   // iterators
   BSTIterator <T> & operator ++ ();
   BSTIterator <T>   operator ++ (int postfix)
   {
      BSTIterator <T> itReturn = *this;
      ++(*this);
      return itReturn;
   }
   BSTIterator <T> & operator -- ();
   BSTIterator <T>   operator -- (int postfix)
   {
      BSTIterator <T> itReturn = *this;
      --(*this);
      return itReturn;
   }

   // must give friend status to remove so it can call getNode() from it
   friend void BST <T> :: remove(BSTIterator <T> & it);

private:
   
   // get the node pointer
   BinaryNode <T> * getNode() { return nodes.top(); }
   
   // the stack of nodes
   Stack < BinaryNode <T> * > nodes;
};


/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <class T>
BSTIterator <T> & BSTIterator <T> :: operator ++ ()
{
   // do nothing if we have nothing
   if (nodes.top() == NULL)
      return *this;
   
   // if there is a right node, take it
   if (nodes.top()->pRight != NULL)
   {
      nodes.push(nodes.top()->pRight);

      // there might be more left-most children
      while (nodes.top()->pLeft)
         nodes.push(nodes.top()->pLeft);
      return *this;
   }

   // there are no right children, the left are done
   assert(nodes.top()->pRight == NULL);
   BinaryNode <T> * pSave = nodes.top();
   nodes.pop();

   // if the parent is the NULL, we are done!
   if (NULL == nodes.top())
      return *this;

   // if we are the left-child, got to the parent.
   if (pSave == nodes.top()->pLeft)
      return *this;

   // we are the right-child, go up as long as we are the right child!
   while (nodes.top() != NULL && pSave == nodes.top()->pRight)
   {
      pSave = nodes.top();
      nodes.pop();
   }
      
   return *this;      
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <class T>
BSTIterator <T> & BSTIterator <T> :: operator -- ()
{
   // do nothing if we have nothing
   if (nodes.top() == NULL)
      return *this;

   // if there is a left node, take it
   if (nodes.top()->pLeft != NULL)
   {
      nodes.push(nodes.top()->pLeft);

      // there might be more right-most children
      while (nodes.top()->pRight)
         nodes.push(nodes.top()->pRight);
      return *this;
   }

   // there are no left children, the right are done
   assert(nodes.top()->pLeft == NULL);
   BinaryNode <T> * pSave = nodes.top();
   nodes.pop();

   // if the parent is the NULL, we are done!
   if (NULL == nodes.top())
      return *this;

   // if we are the right-child, got to the parent.
   if (pSave == nodes.top()->pRight)
      return *this;

   // we are the left-child, go up as long as we are the left child!
   while (nodes.top() != NULL && pSave == nodes.top()->pLeft)
   {
      pSave = nodes.top();
      nodes.pop();
   }

   return *this;
}


#endif // BST_H
