//
//  RBTreeStdIterator.hpp
//  RBTree
//
//  Created by Александр Пахомов on 22.05.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef RBTreeStdIterator_hpp
#define RBTreeStdIterator_hpp

class const_iterator{
protected:
	CRootRBNode* _root;
	CRBNode* _node;
	bool _fIsAfterLastNode;
	bool _fIsPrevFirstNode;
	
	void findNext() {
		if(!_fIsAfterLastNode){
			if(_fIsPrevFirstNode){
				_node = findMin(_root);
				if(_node == nullptr){
					_fIsAfterLastNode = true;
				}
				_fIsPrevFirstNode = false;
			}
			else{
				_node = findNextNode(_root, _node);
				if(_node == nullptr){
					_fIsAfterLastNode = true;
				}
			}
		}
	}
	void findPrev(){
		if(!_fIsPrevFirstNode){
			if(_fIsAfterLastNode){
				_node = findMax(_root);
				if(_node != nullptr){
					_fIsAfterLastNode = false;
				}
			}
			else{
				_node = findPrevNode(_root, _node);
				if(_node == nullptr){
					_fIsPrevFirstNode = true;
				}
			}
		}
	}
	
	typedef const_iterator self_type;
	
public:
	const_iterator(CRBNode* root_node_pointer,
				   CRootRBNode* node_pointer,
				   const bool fIsPrevFirst = false,
				   const bool fIsAfterLast = false) :
	_root(root_node_pointer),
	_node(node_pointer),
	_fIsAfterLastNode(fIsAfterLast || node_pointer == nullptr),
	_fIsPrevFirstNode(fIsPrevFirst) {}
	
	self_type operator++(int junk) { self_type it = *this; findNext(); return it; }
	self_type operator++() { findNext(); return *this; }
	
	self_type operator--(int junk) { self_type it = *this; findPrev(); return it; }
	self_type operator--() { findPrev(); return *this; }
	
	const value_type& operator*() { return _node->getValue(); }
	const value_type* operator->() { return _node->getValuePointer(); }
	
	bool operator==(const self_type& other) {
		if(_fIsPrevFirstNode ^ other._fIsPrevFirstNode ||
		   _fIsAfterLastNode ^ other._fIsAfterLastNode)
			return false;
		
		else return
			(_fIsPrevFirstNode && other._fIsPrevFirstNode) ||
			(_fIsAfterLastNode && other._fIsAfterLastNode) ||
			(*_node == *other._node); }
	
	bool operator!=(const self_type& other) { return !(*this == other); }
	
	bool fIsEndIterator() const { return (_fIsAfterLastNode); }
	
};

#endif /* RBTreeStdIterator_hpp */
