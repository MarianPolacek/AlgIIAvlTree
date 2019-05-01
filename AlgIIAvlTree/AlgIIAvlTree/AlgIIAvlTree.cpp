// AlgIIAvlTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include <iostream>
#include "CAVLTree.h"

int main()
{
	CAVLTree<int> intTree;
	
	intTree.Insert(4);
	intTree.Insert(5);
	intTree.Insert(7);
	intTree.Insert(2);
	intTree.Insert(1);
	intTree.Insert(3);
	intTree.Insert(6);

	CAVLTree<char> charTree;

	charTree.Insert('A');
	charTree.Insert('B');
	charTree.Insert('C');
	charTree.Insert('D');
	charTree.Insert('E');
	charTree.Insert('F');
	charTree.Insert('G');

	CAVLTree<int> deleteCheckTree;

	deleteCheckTree.Insert(5);
	deleteCheckTree.Insert(3);
	deleteCheckTree.Insert(8);
	deleteCheckTree.Insert(2);
	deleteCheckTree.Insert(4);	
	deleteCheckTree.Insert(7);
	deleteCheckTree.Insert(10);
	deleteCheckTree.Insert(1);
	deleteCheckTree.Insert(6);
	deleteCheckTree.Insert(9);
	deleteCheckTree.Insert(11);

	deleteCheckTree.Delete(4);
	deleteCheckTree.Delete(8);
	deleteCheckTree.Delete(6);
	deleteCheckTree.Delete(5);
	deleteCheckTree.Delete(2);
	deleteCheckTree.Delete(1);
	deleteCheckTree.Delete(7);

	system("pause");
}