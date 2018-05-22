#include "CRBTree.hpp"
#include <iostream>
using namespace std;
int main() {
	CRBTree tree;
	tree.insert(1);
	tree.insert(2);
	tree.insert(4);
	tree.insert(5);
	tree.insert(6);
	
	//tree.remove(1);
	tree.printBlackHeight();
	cout << endl;
	//tree.remove(4);
	tree.printBlackHeight();
	cout << endl;
	//tree.remove(6);
	tree.printBlackHeight();
	cout << endl;
	
	tree.printBlackHeight();
	cout << endl;
	
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
	
	for(int c = 0; c < 10; ++c){
		int n = 10000 + rand() % 10000;
		for(int i = 0; i < n; ++i){
			tree.insert(rand() % 100000);
		}
	
	
		for(int i = 0; i < n *30; ++i){
			int iRem = rand() % 100000;
			tree.remove(iRem);
		}
	}
	
	
	cout << "Final check of RBTree" << endl;
	tree.printBlackHeight();
	cout << endl;
	
	cout << "Check of property 2 : " << tree.checkIfRootIsBlack() << endl;
	cout << "Check of preperty 4 : " << tree.checkProperty4() << endl;

	return 0;
}
