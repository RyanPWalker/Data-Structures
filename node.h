
#ifndef Node_H
#define Node_H

#include <cassert>
#include <iostream>

using namespace std;
 
/************************************************
 * Node
 * Data Structure that implements a linked-list.
 ***********************************************/
template <class T>
class Node
{
public:

   T data;				// holds the data
   Node<T>* pNext;	// points to the next node

   // default constructor : empty and kinda useless
   Node() : pNext(NULL) {};
   
   // non-default constructor : create a new node with the data
   Node(T nData) { this->data = nData; pNext = NULL; }
};

/***************************************************
 * Node :: copy
 * Receives a node, copys it, and returns the copy
 **************************************************/
template <class T>
Node <T> * copy(Node <T> *node)
{
	Node<T> *copy = new Node<T>;
	copy->data = node->data;
	copy->pNext = node->pNext;
	Node<T> *head = copy; // We do not loop through yet because we want to keep track of the head
	node = node->pNext;

	// Now we can loop through
	while(node)
	{
		copy->pNext = new Node<T>;
		copy = copy->pNext;
		copy->data = node->data;
		node = node->pNext;
	}

	// return the head of the node and not anything inbetween
	// otherwise we lose part of the list
	return head;
}

/***************************************************
 * Node :: INSERT
 * Insert a node inside of the linked-list
 **************************************************/
template <class T>
void insert(T nData, Node<T> *&prev, bool head = false)
{
	// this is the node that will be inserted
   Node <T>* nNode = new Node<T>(nData);

   if (head || prev == NULL)
   {
   	nNode->pNext = prev;
   	prev = nNode;
   }
   else
   {
   	nNode->pNext = prev->pNext;
   	prev->pNext = nNode;
   }
}

/***************************************************
 * Node :: find
 * Takes a template parameter and returns an node
 * with the item in the linked-list.  If the item is
 * not found, it returns NULL.
 **************************************************/
template <class T>
Node <T> * find(Node<T>* head, const T & get)
{
   while(head != NULL)
   {
   	if (head->data == get)
   		return head;
   	else
   		head = head->pNext;
   }

   return head;
}

/***************************************************
 * Node :: freeData
 * Takes a Node parameter and will recursively free
 * up all the space in the linked list.
 **************************************************/
template <class T>
void freeData(Node<T> *&n)
{
	if (n != NULL) // if there's data, delete
	{
		freeData(n->pNext);
		delete n;
		n = NULL;
	}
	else
	{
		return; // anchor
	}

}

/******************************************
 * NODE :: insertion operator
 * Display node data
 *****************************************/
template <class T>
ostream & operator << (ostream & out, const Node <T> *node)
{
   while(node)
   {
   	out << node->data;
   	if (node->pNext)
   	{
   		out << ", ";
   	}

   	node = node->pNext;
   }

   return out;
}

#endif // Node_H