#include <string>
#include <iostream>


#include "CRBTree.hpp"
using namespace std;

int main(){
	
	CRBTree<int, greater<int>> bst;
	
	srand(clock());
	for(int i = 0; i < 20; ++i) {
		bst.insert(rand());
	}
	
	for(auto it: bst) {
		cout << it << endl;
	}
	
	cout << endl << "Traversal Preorder Deep First" << endl;
	for(auto it = bst.beginPreorderDF(); it != bst.end(); ++it) {
		cout << *it << endl;
	}
	
	cout << endl << "Traversal Inorder Deep First" << endl;
	for(auto it = bst.beginInorderDF(); it != bst.end(); ++it) {
		cout << *it << endl;
	}
	
	cout << endl << "Traversal Postorder Deep First" << endl;
	for(auto it = bst.beginPostorderDF(); it != bst.end(); ++it) {
		cout << *it << endl;
	}
	
	bst.insert(1);
	bst.remove(1);
	bst.begin();
	bst.beginBF();
	bst.beginInorderDF();
	bst.beginPostorderDF();
	bst.beginPreorderDF();
	bst.end();
	bst.empty();
	bst.size();
	bst.find(1);
	bst.clear();
	
    return 0;
}










