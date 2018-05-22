#ifndef CRBTree_h
#define CRBTree_h

#include <iostream>
#include <queue>
using namespace std;

#define is_leaf(n) ((n) == nullptr ? true : false)



template<class value_type, class comporator = std::less<value_type>>
class CRBTree{
public:
	
#include "CRBTreeNode.hpp"
	
#include "CRBTreeStdIterator.hpp"
	
#include "CRBTraversalIterators.hpp"
	
	CRBTree() :
	_root(nullptr) { }
	
	void insert(const value_type& value) {
		_root = insert_node(value);
	}
	
	void remove(const value_type& value){
		CRBNode* n = findNode(value);
		remove_node(n);
	}
	
	const_iterator begin() const { return const_iterator(_root, findMin(_root)); }
	iteratorPreorderDF beginPreorderDF() const { return iteratorPreorderDF(_root); }
	iteratorInorderDF beginInorderDF() const { return iteratorInorderDF(_root); }
	iteratorPostorderDF beginPostorderDF() const { return iteratorPostorderDF(_root); }
	iteratorBF beginBF() const { return iteratorBF(_root); }
	
	const_iterator end() const { return const_iterator(_root, nullptr, false, true); }
	const_iterator find(const value_type& value) const { return const_iterator(_root, findNode(value)); }
	
	CRBNode* findNode(const value_type& value) const {
		CRBNode* crt = _root;
		CRBNode valueNode(value);
		while(crt != nullptr){
			if(valueNode > (*crt)){
				crt = crt->getRightChild();
			}
			else{
				if(valueNode != *crt){
					crt = crt->getLeftChild();
				}
				else{
					break;
				}
			}
		}
		return crt;
	}
	
	void printBlackHeight() {
		crtBlackHeight = 0;
		recPrintBlackHeight(_root);
	}
	
	bool checkIfRootIsBlack() const {
		return _root->isBlack();
	}
	
	bool checkProperty4() const {
		return check4(false, _root);
	}
	
	const CRootRBNode* getRoot() { return _root; }
	
private:
	
	bool check4(bool fLastWasRed, CRBNode* n) const {
		if(n == nullptr)
			return true;
		if(fLastWasRed){
			if(n->isRed()){
				return false;
			}
		}
		bool isRed = n->isRed();
		return check4(isRed, n->getLeftChild());
		return check4(isRed, n->getRightChild());
	}
	
	CRootRBNode* insert_node(const value_type& value) {
		CRootRBNode* root = _root;
		CRBNode* crt = root;
		CRBNode* new_node = nullptr;
		if(root == nullptr){
			root = new CRootRBNode(value);
			new_node = root;
		}
		else{
			while(true){
				if((*crt) < value){
					if(crt->fHasRightChild()){
						crt = crt->getRightChild();
					}
					else{
						crt->setRightChild(new_node = new CRBNode(value, crt));
						break;
					}
				}
				else{
					if((*crt) != value){
						if(crt->fHasLeftChild()){
							crt = crt->getLeftChild();
						}
						else{
							crt->setLeftChild(new_node = new CRBNode(value, crt));
							break;
						}
					}
					else{
						break;
					}
				}
			}
		}
		if(new_node != nullptr){
			root = insert_case1(root, new_node);
		}
		return root;
	}
	
	static CRootRBNode* insert_case1(CRootRBNode* root, CRBNode* n) {
		if(n->isRoot()){
			n->setBlack();
		}
		else{
			root = insert_case2(root, n);
		}
		return root;
	}
	
	static CRootRBNode* insert_case2(CRootRBNode* root, CRBNode* n) {
		// parent can't be null
		CParentRBNode* parent = n->getParent();
		if(parent->isRed()){
			root = insert_case3(root, n);
		}
		return root;
	}
	
	static CRootRBNode* insert_case3(CRootRBNode* root, CRBNode* n) {
		// parent is red
		CUncleRBNode* uncle = n->getUncle();
		CGrandparentRBNode* grandparent = n->getGrandParent();
		CParentRBNode* parent = n->getParent();
		
		if(uncle != nullptr && uncle->isRed()){
			parent->setBlack();
			uncle->setBlack();
			grandparent->setRed();
			root = insert_case1(root, grandparent);
		}
		else{
			root = insert_case4(root, n);
		}
		return root;
	}
	
	static CRootRBNode* insert_case4(CRootRBNode* root, CRBNode* n) {
		CParentRBNode* parent = n->getParent();
		
		if(n->isRightChild() && parent->isLeftChild()){
			if(parent->isRoot()){
				root = parent->leftRotate();
			}
			else parent->leftRotate();
			n = n->getLeftChild();
		}
		else if( n->isLeftChild() && parent->isRightChild()){
			if(parent->isRoot()){
				root = parent->rightRotate();
			}
			else parent->rightRotate();
			n = n->getRightChild();
		}
		root = insert_case5(root, n);
		return root;
	}
	
	static CRootRBNode* insert_case5(CRootRBNode* root, CRBNode* n) {
		CGrandparentRBNode* grandparent = n->getGrandParent();
		CParentRBNode* parent = n->getParent();
		
		parent->setBlack();
		grandparent->setRed();
		if(n->isLeftChild() && parent->isLeftChild()){
			if(grandparent->isRoot()){
				root = grandparent->rightRotate();
			}
			else grandparent->rightRotate();
		}
		else{
			if(grandparent->isRoot()){
				root = grandparent->leftRotate();
			}
			else grandparent->leftRotate();
		}
		return root;
	}
	
	
	
	void remove_node(CRBNode* n) {
		if(n != nullptr)
			_root = remove_node_by_pointer(_root, n);
	}
	
	CRootRBNode* remove_node_by_pointer(CRootRBNode* root, CRBNode* n) {
		
		if(n->fHasLeftChild() && n->fHasRightChild()){
			CRBNode* next = findNextNode(root, n);
			const value_type value = n->getValue();
			n->setValue(next->getValue());
			next->setValue(value);
			n = next;
		}
		
		CRBNode* child = is_leaf(n->getLeftChild()) ? n->getRightChild() : n->getLeftChild();
		CParentRBNode* parent = n->getParent();
		bool isLeft = n->isLeftChild();
		bool isRoot = n->isRoot();
		
		n->replace_with(child);
		if(n->isBlack()){
			if(child != nullptr && child->isRed()){
				if(isRoot){
					root = child;
				}
				child->setBlack();
			}
			else{
				root = delete_case1(root, parent, isLeft, child);
			}
		}
		delete n;
		
		return root;
	}
	
	static CRootRBNode* delete_case1(CRootRBNode* root, CParentRBNode* parent, bool isLeft, CRBNode* n){
		if(parent != nullptr){
			root = delete_case2(root, parent, isLeft, n);
		}
		else{
			root = n;
		}
		return root;
	}
	
	static CRootRBNode* delete_case2(CRootRBNode* root, CParentRBNode* parent, bool isLeft, CRBNode* n){
		CBrotherRBNode* brother = isLeft ? parent->getRightChild() : parent->getLeftChild();
		
		if(brother != nullptr && brother->isRed()){
			parent->setRed();
			brother->setBlack();
			if(isLeft){
				if(parent->isRoot())
					root = parent->leftRotate();
				else
					parent->leftRotate();
			}
			else{
				if(parent->isRoot())
					root = parent->rightRotate();
				else
					parent->rightRotate();
			}
		}
		root = delete_case3(root, parent, isLeft, n);
		return root;
	}
	
	static CRootRBNode* delete_case3(CRootRBNode* root, CParentRBNode* parent, bool isLeft, CRBNode* n){
		CBrotherRBNode* brother = isLeft ? parent->getRightChild() : parent->getLeftChild();

		if(brother != nullptr){
			CLeftChildRBNode* left = brother->getLeftChild();
			CRightChildRBNode* right = brother->getRightChild();
			if(parent->isBlack() && brother->isBlack() &&
			   left->isBlack() && right->isBlack()){
				brother->setRed();
				root = delete_case1(root, parent->getParent(), parent->isRoot() ? false : parent->isLeftChild(), parent);
			}
			else{
				root = delete_case4(root, parent, isLeft, n);
			}
		}
		else{
			root = delete_case4(root, parent, isLeft, n);
		}
		return root;
	}
	
	static CRootRBNode* delete_case4(CRootRBNode* root, CParentRBNode* parent, bool isLeft, CRBNode* n){
		CBrotherRBNode* brother = isLeft ? parent->getRightChild() : parent->getLeftChild();

		CLeftChildRBNode* left = brother->getLeftChild();
		CRightChildRBNode* right = brother->getRightChild();
		if(brother != nullptr && parent->isRed() && brother->isBlack() &&
			left->isBlack() && right->isBlack()){
			brother->setRed();
			parent->setBlack();
		}
		else {
			root = delete_case5(root, parent, isLeft, n);
		}
		return root;
	}
	
	static CRootRBNode* delete_case5(CRootRBNode* root, CParentRBNode* parent, bool isLeft, CRBNode* n){
		CBrotherRBNode* brother = isLeft ? parent->getRightChild() : parent->getLeftChild();

		if(brother != nullptr && brother->isBlack()){
			CLeftChildRBNode* left = brother->getLeftChild();
			CRightChildRBNode* right = brother->getRightChild();
			
			if(isLeft && right->isBlack() && left->isRed()){
				brother->setRed();
				left->setBlack();
				if(brother->isRoot())
					root = brother->rightRotate();
				else
					brother->rightRotate();
			}
			else if(!isLeft && left->isBlack() && right->isRed()){
				brother->setRed();
				right->setBlack();
				if(brother->isRoot())
					root = brother->leftRotate();
				else
					brother->leftRotate();
			}
		}
		root = delete_case6(root, parent, isLeft, n);
		return root;
	}
	
	static CRootRBNode* delete_case6(CRootRBNode* root, CParentRBNode* parent, bool isLeft, CRBNode* n){
		CBrotherRBNode* brother = isLeft ? parent->getRightChild() : parent->getLeftChild();

		if(parent->isRed()){
			brother->setRed();
		}
		else{
			brother->setBlack();
		}
		parent->setBlack();
		
		CLeftChildRBNode* left = brother->getLeftChild();
		CRightChildRBNode* right = brother->getRightChild();
		if(isLeft){
			right->setBlack();
			if(parent->isRoot())
				root = parent->leftRotate();
			else
				parent->leftRotate();
		}
		else{
			left->setBlack();
			if(parent->isRoot())
				root = parent->rightRotate();
			else
				parent->rightRotate();
		}
		return root;
	}
	
	
	static CRBNode* findMax(CRBNode* root) {
		CRBNode* crt = root;
		if(crt != nullptr){
			while(crt->fHasRightChild()){
				crt = crt->getRightChild();
			}
		}
		return crt;
	}
	
	static CRBNode* findMin(CRBNode* root) {
		CRBNode* crt = root;
		if(crt != nullptr){
			while(crt->fHasLeftChild()){
				crt = crt->getLeftChild();
			}
		}
		return crt;
	}

	static CRBNode* findNextNode(CRBNode* root, CRBNode* start) {
		CRBNode* crt = nullptr;
		if(start->fHasRightChild()){
			crt = findMin(start->getRightChild());
		}
		else{
			crt = start->getParent();
			while(crt != nullptr && start == crt->getRightChild()){
				start = crt;
				crt = crt->getParent();
			}
		}
		return crt;
	}
	
	static CRBNode* findPrevNode(CRBNode* root, CRBNode* start) {
		CRBNode* crt = nullptr;
		if(start->fHasLeftChild()){
			crt = findMax(start->getLeftChild());
		}
		else{
			crt = start->getParent();
			while(crt != nullptr && start == crt->getLeftChild()){
				start = crt;
				crt = crt->getParent();
			}
		}
		return crt;
	}
	
	
	
	void recPrintBlackHeight(CRBNode* crt) {
		if(crt == nullptr){
			return;
		}
		if(crt->isBlack()){
			++crtBlackHeight;
		}
		if(!crt->fHasLeftChild() && !crt->fHasRightChild()){
			cout << crt->getValue() << "\tbh == " << crtBlackHeight << endl;
		}
		recPrintBlackHeight(crt->getRightChild());
		
		recPrintBlackHeight(crt->getLeftChild());
		
		if(crt->isBlack()){
			--crtBlackHeight;
		}
	}
	
protected:
	CRootRBNode* _root;
	int crtBlackHeight;
};

#endif /* CRBTree_h */





















