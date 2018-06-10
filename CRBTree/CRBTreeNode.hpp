//
//  CRBTreeNode.hpp
//  RBTree
//
//  Created by Александр Пахомов on 22.05.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef CRBTreeNode_hpp
#define CRBTreeNode_hpp

class CRBNode;

typedef CRBNode CRootRBNode;
typedef CRBNode CParentRBNode;
typedef CRBNode CUncleRBNode;
typedef CRBNode CGrandparentRBNode;
typedef CRBNode CLeftChildRBNode;
typedef CRBNode CRightChildRBNode;
typedef CRBNode CBrotherRBNode;

enum ERBNodeColor { red, black };

class CRBNode{
public:
	CRBNode(const value_type& value,
			CParentRBNode* parent = nullptr,
			CLeftChildRBNode* left_child = nullptr,
			CRightChildRBNode* right_child = nullptr,
			ERBNodeColor color = red) :
	_value(value),
	_parent(parent),
	_left_child(left_child),
	_right_child(right_child),
	_color(color) { }
	
	
	
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
	
	bool isLeftChild() const { return ( _parent != nullptr && _parent->_left_child == this ); }
	bool isRightChild() const { return ( _parent!= nullptr && _parent->_right_child == this); }
	bool isNullLeaf() const { return ( this == nullptr ); }
	
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
			if(!isRoot()){
				if(isLeftChild()){
					_parent->_left_child = n;
				}
				else{
					_parent->_right_child = n;
				}
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
	
	bool operator>(const value_type& value) { comporator comp; return comp( _value, value ); }
	bool operator==(const value_type& value) { return _value == value; }
	
	bool operator<(const value_type& value) { return (!( *this > value ) && ( *this != value ) ); }
	bool operator!=(const value_type& value) { return !( *this == value ); }
	bool operator>=(const value_type& value) { return !( *this < value ); }
	bool operator<=(const value_type& value) { return !( *this > value ); }
	
	
	bool operator>(const CRBNode& n) { comporator comp; return comp( _value, n._value ); }
	bool operator==(const CRBNode& n) { return ( _value == n._value ); }
	
	bool operator<(const CRBNode& n) { return ( !( *this > n ) && ( *this != n ) ); }
	bool operator!=(const CRBNode& n) { return !( *this == n ); }
	bool operator>=(const CRBNode& n) { return !( *this < n ); }
	bool operator<=(const CRBNode& n) { return !( *this > n ); }
	
protected:
	value_type _value;
	ERBNodeColor _color;
	
	CParentRBNode* _parent;
	CLeftChildRBNode* _left_child;
	CRightChildRBNode* _right_child;
};

#endif /* CRBTreeNode_hpp */
