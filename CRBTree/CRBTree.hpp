#ifndef CRBTree_h
#define CRBTree_h

#include <iostream>
using namespace std;

#define is_leaf(n) ((n) == nullptr ? true : false)

typedef int value_type;

class CRBNode;

typedef CRBNode CRootRBNode;
typedef CRBNode CParentRBNode;
typedef CRBNode CUncleRBNode;
typedef CRBNode CGrandparentRBNode;
typedef CRBNode CLeftChildRBNode;
typedef CRBNode CRightChildRBNode;
typedef CRBNode CBrotherRBNode;

class CRBNode{
public:
	CRBNode(const value_type& value,
		  CParentRBNode* parent = nullptr,
		  CLeftChildRBNode* left_child = nullptr,
		  CRightChildRBNode* right_child = nullptr) :
	_value(value),
	_parent(parent),
	_left_child(left_child),
	_right_child(right_child),
	_color(red) { }
	
	const value_type& getValue() const { return _value; }
	
	void setValue(const value_type& value) { _value = value; }
	void setParent(CRBNode* parent) { _parent = parent; }
	void setLeftChild(CRBNode* left_child) { _left_child = left_child; }
	void setRightChild(CRBNode* right_child) { _right_child = right_child; }
	
	CParentRBNode* getParent() const { return this != nullptr ? _parent : nullptr; }
	CLeftChildRBNode* getLeftChild() const { return this != nullptr ? _left_child : nullptr; }
	CRightChildRBNode* getRightChild() const { return this != nullptr ? _right_child : nullptr; }
	
	bool isRoot() const { return ( _parent == nullptr ); }
	bool fHasLeftChild() const { return ( _left_child != nullptr ); }
	bool fHasRightChild() const { return ( _right_child != nullptr ); }
	
	bool isRed() const { return ( this != nullptr && _color == red ); }
	bool isBlack() const { return ( this == nullptr || _color == black ); }
	
	void setRed() { _color = red; }
	void setBlack() { if(this != nullptr) _color = black; }
	
	bool isLeftChild() const { return ( _parent->_left_child == this ); }
	bool isRightChild() const { return ( _parent->_right_child == this); }
	
	CGrandparentRBNode* getGrandParent() const { return ( _parent == nullptr ? nullptr : _parent->_parent ); }
	CUncleRBNode* getUncle() const {
		CGrandparentRBNode* grand = getGrandParent();
		CUncleRBNode* uncle = nullptr;
		if(grand != nullptr){
			if(_parent->isLeftChild()){
				uncle = grand->_right_child;
			}
			else{
				uncle = grand->_left_child;
			}
		}
		return uncle;
	}
	CBrotherRBNode* getBrother() const {
		if(_parent == nullptr)
			return nullptr;
		if(isLeftChild()){
			return _parent->_right_child;
		}
		else{
			return _parent->_left_child;
		}
	}
	// left rotate return pivot
	CRBNode* leftRotate() {
		CRBNode* pivot = _right_child;
		
		pivot->_parent = _parent;
		if(_parent != nullptr){
			if(isLeftChild()){
				_parent->_left_child = pivot;
			}
			else{
				_parent->_right_child = pivot;
			}
		}
		
		_right_child = pivot->_left_child;
		if(pivot->_left_child != nullptr){
			pivot->_left_child->_parent = this;
		}
		_parent = pivot;
		pivot->_left_child = this;
		return pivot;
	}
	// right rotate return pivot
	CRBNode* rightRotate() {
		CRBNode* pivot = _left_child;
		
		pivot->_parent = _parent;
		if(_parent != nullptr){
			if(isLeftChild()){
				_parent->_left_child = pivot;
			}
			else{
				_parent->_right_child = pivot;
			}
		}
		_left_child = pivot->_right_child;
		if(_left_child != nullptr){
			_left_child->_parent = this;
		}
		_parent = pivot;
		pivot->_right_child = this;
		return pivot;
	}
	
	void replace_with(CRBNode* n) {
		if(this != n){
			if(isLeftChild()){
				_parent->_left_child = n;
			}
			else{
				_parent->_right_child = n;
			}
			if(n != nullptr){
				n->_parent = _parent;
				_parent = n;
				n->_left_child = _left_child;
				n->_right_child = _right_child;
				if(n->_left_child == n)
					n->_left_child = nullptr;
				if(n->_right_child == n)
					n->_right_child = nullptr;
			}
		}
	}
	
	bool operator>(const value_type& value) { return _value > value; }
	bool operator==(const value_type& value) { return _value == value; }
	
	bool operator<(const value_type& value) { return (!( *this > value ) && ( *this != value ) ); }
	bool operator!=(const value_type& value) { return !( *this == value ); }
	bool operator>=(const value_type& value) { return !( *this < value ); }
	bool operator<=(const value_type& value) { return !( *this > value ); }
	
	
	bool operator>(const CRBNode& n) { return ( _value > n._value ); }
	bool operator==(const CRBNode& n) { return ( this == &n ); }
	
	bool operator<(const CRBNode& n) { return ( !( *this > n ) && ( *this != n ) ); }
	bool operator!=(const CRBNode& n) { return !( *this == n ); }
	bool operator>=(const CRBNode& n) { return !( *this < n ); }
	bool operator<=(const CRBNode& n) { return !( *this > n ); }
	
protected:
	value_type _value;
	enum EColor { red, black } _color;
	
	CParentRBNode* _parent;
	CLeftChildRBNode* _left_child;
	CRightChildRBNode* _right_child;
};


class CRBTree{
public:
	CRBTree() :
	_root(nullptr) { }
	
	void insert(const value_type& value) {
		_root = insert_node(value);
	}
	
	void remove(const value_type& value){
		CRBNode* n = findNode(value);
		remove_node(n);
	}
	
	CRBNode* findNode(const value_type& value){
		CRBNode* crt = _root;
		while(crt != nullptr){
			if(value > crt->getValue()){
				crt = crt->getRightChild();
			}
			else{
				if(value != crt->getValue()){
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
	
	CRootRBNode* insert_case1(CRootRBNode* root, CRBNode* n) {
		if(n->isRoot()){
			n->setBlack();
		}
		else{
			root = insert_case2(root, n);
		}
		return root;
	}
	
	CRootRBNode* insert_case2(CRootRBNode* root, CRBNode* n) {
		// parent can't be null
		CParentRBNode* parent = n->getParent();
		if(parent->isRed()){
			root = insert_case3(root, n);
		}
		return root;
	}
	
	CRootRBNode* insert_case3(CRootRBNode* root, CRBNode* n) {
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
	
	CRootRBNode* insert_case4(CRootRBNode* root, CRBNode* n) {
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
	
	CRootRBNode* insert_case5(CRootRBNode* root, CRBNode* n) {
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
		
		n->replace_with(child);
		if(n->isBlack()){
			if(child != nullptr && child->isRed()){
				child->setBlack();
			}
			else{
				root = delete_case1(root, parent, isLeft, child);
			}
		}
		delete n;
		
		return root;
	}
	
	CRootRBNode* delete_case1(CRootRBNode* root, CParentRBNode* parent, bool isLeft, CRBNode* n){
		if(parent != nullptr){
			root = delete_case2(root, parent, isLeft, n);
		}
		return root;
	}
	
	CRootRBNode* delete_case2(CRootRBNode* root, CParentRBNode* parent, bool isLeft, CRBNode* n){
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
	
	CRootRBNode* delete_case3(CRootRBNode* root, CParentRBNode* parent, bool isLeft, CRBNode* n){
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
	
	CRootRBNode* delete_case4(CRootRBNode* root, CParentRBNode* parent, bool isLeft, CRBNode* n){
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
	
	CRootRBNode* delete_case5(CRootRBNode* root, CParentRBNode* parent, bool isLeft, CRBNode* n){
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
	
	CRootRBNode* delete_case6(CRootRBNode* root, CParentRBNode* parent, bool isLeft, CRBNode* n){
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
	
private:
	CRootRBNode* _root;
	int crtBlackHeight;
};

#endif /* CRBTree_h */





















