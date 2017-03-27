#ifndef BNODE_H
#define BNODE_H

#include <iostream>
#include <cassert>

using namespace std;

/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree.  Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *
 * Author: Ryan Walker
 *****************************************************************/
template <class T>
class BinaryNode
{
public:
	// data inside nodes
   T data;
   // left node.  Points to its own parent and potentially its own left and right nodes
   BinaryNode <T> * pLeft;
   // right node.  Points to its own parent and potentially its own left and right nodes
   BinaryNode <T> * pRight;
   // parent node.  Points to left and right
   BinaryNode <T> * pParent;

   // default constructor : empty and kinda useless
   BinaryNode() : data(NULL), pLeft(NULL), pRight(NULL), pParent(NULL) {}

   // non-default constructor : takes a template as a parameter
   // and creates a root node
   BinaryNode(T data)
   {
   	this->data = data;
   	pLeft   = NULL;
   	pRight  = NULL;
   	pParent = NULL;
   }

   // return size (i.e. number of nodes in tree)
   int size() const
   {
      return 1 +
         (pLeft  == NULL ? 0 : pLeft->size()) +
         (pRight == NULL ? 0 : pRight->size());
   }

   // add a node the left/right
   BinaryNode <T> addLeft (BinaryNode <T> * pNode);
   BinaryNode <T> addRight(BinaryNode <T> * pNode);

   // create a node and add it to the left/right
   BinaryNode <T> addLeft (const T & t) throw (const char *);
   BinaryNode <T> addRight(const T & t) throw (const char *);
};

/***************************************************
 * BinaryNode :: addLeft(BinaryNode <T> * pNode)
 * Takes a node as a paramter and links it to the
 * binary tree through it's current parent on the left.
 **************************************************/
template <class T>
BinaryNode <T> BinaryNode <T> :: addLeft (BinaryNode <T> * pNode)
{
	pLeft = pNode;
	if (pLeft != NULL)
	{
		pLeft->pParent = this;
	}

	return *this;
}

/***************************************************
 * BinaryNode :: addRight(BinaryNode <T> * pNode)
 * Takes a node as a paramter and links it to the
 * binary tree through it's current parent on the right.
 **************************************************/
template <class T>
BinaryNode <T> BinaryNode <T> :: addRight(BinaryNode <T> * pNode)
{
	pRight = pNode;
	if (pRight != NULL)
	{
		pRight->pParent = this;
	}

	return *this;
}

/***************************************************
 * BinaryNode :: addLeft(const T & t)
 * Takes a template as a paramter, creates a new node
 * with the template data, and then links the new node to
 * the binary tree through it's current parent on the left.
 **************************************************/
template <class T>
BinaryNode <T> BinaryNode <T> :: addLeft (const T & t) throw (const char *)
{
	pLeft = new BinaryNode<T>(t);
	pLeft->pParent = this;

	return *this;
}

/***************************************************
 * BinaryNode :: addRight(const T & t)
 * Takes a template as a paramter, creates a new node
 * with the template data, and then links the new node to
 * the binary tree through it's current parent on the right.
 **************************************************/
template <class T>
BinaryNode <T> BinaryNode <T> :: addRight(const T & t) throw (const char *)
{
	pRight = new BinaryNode<T>(t);
	pRight->pParent = this;

	return *this;
}

/******************************************
 * insertion operator (NON-MEMBER FUNCTION)
 * Recursively displays node data
 *****************************************/
template <class T>
ostream & operator <<(ostream & out, const BinaryNode <T> *node)
{
   if (node->pLeft)
   	cout << node->pLeft;
   out << node->data;
   out << " ";

   if (node->pRight)
   	cout << node->pRight;

   return out;
}

/******************************************
 * deleteBinaryTree (NON-MEMBER FUNCTION)
 * Recursively deletes node data in tree
 *****************************************/
template <class T>
void deleteBinaryTree(BinaryNode <T> *& pNode)
{
	// anchor
   if (pNode == NULL) 
      return;

   // delete from left to right
   deleteBinaryTree(pNode->pLeft);
   deleteBinaryTree(pNode->pRight);
   delete pNode;
}

#endif // BNODE_H
