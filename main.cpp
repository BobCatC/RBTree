#include "CRBTree/CRBTree.hpp"
#include <iostream>
using namespace std;
#define PrintTree() { for(auto it : tree){ cout << it << ' '; } cout << endl; }

int main() {
	CRBTree<int, greater<int>> tree;
	
	for(auto it = tree.begin(); it != tree.end(); ++it){
		cout << *it;
	}
	
	auto itFind = tree.find(12);
	
	tree.insert(0);
	tree.insert(20);
	tree.insert(-20);
	tree.insert(10);
	tree.insert(-10);
	tree.insert(-30);
	tree.insert(30);
	tree.remove(0);
	tree.remove(0);
	tree.remove(1);
	
	itFind = tree.find(30);
	
	PrintTree();
	tree.printBlackHeight();
	cout << endl;
	
	tree.insert(1);;
	tree.insert(2);;
	tree.insert(4);;
	tree.insert(5);;
	tree.insert(6);;
	tree.insert(-2);;
	tree.insert(88);;
	tree.insert(0);;
	tree.insert(-1);;
	tree.insert(0);;
	tree.insert(88);;
	tree.insert(-10);;
	tree.insert(-5);;
	tree.insert(-4);;
	tree.insert(-3);;
	
	PrintTree();
	
	tree.remove(111);
	
	PrintTree();
	
	tree.remove(0);
	tree.remove(88);
	tree.remove(6);
	tree.remove(-5);
	
	PrintTree();
	
	tree.insert(-1);
	tree.insert(3);
	tree.insert(7);
	tree.insert(7);
	
	tree.printBlackHeight();
	cout << endl;
	
	tree.insert(8);
	tree.insert(11);
	tree.insert(12);
	
	tree.insert(9);
	tree.printBlackHeight();
	cout << endl;
	tree.insert(10);
	tree.insert(-6);
	tree.insert(-7);
	tree.insert(-9);
	tree.insert(-8);
	tree.insert(-10);
	tree.insert(-20);
	tree.printBlackHeight();
	cout << endl;
	tree.insert(-19);
	tree.insert(-18);
	
	tree.remove(-9);
	tree.remove(-20);
	tree.remove(10);
	tree.remove(-10);
	srand(clock());
	for(int c = 0; c < 10; ++c){
		int n = 100 + rand() % 100;
		for(int i = 0; i < n; ++i){
			tree.insert(rand() % 1000 - 100);
		}
	
	
		for(int i = 0; i < n * 5; ++i){
			int iRem = rand() % 1000 - 100;
			tree.remove(iRem);
		}
	}
	
	
	cout << "Final check of RBTree" << endl;
	PrintTree();
	tree.printBlackHeight();
	cout << endl;
	
	cout << "Check of property 2 : " << tree.checkIfRootIsBlack() << endl;
	cout << "Check of preperty 4 : " << tree.checkProperty4() << endl;

	return 0;
}
