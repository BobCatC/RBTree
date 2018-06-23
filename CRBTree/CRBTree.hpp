//  RBTree
//
//  Created by Александр Пахомов on 22.05.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved (no).
//

#ifndef CRBTree_h
#define CRBTree_h

#include <iostream>
#include <queue>


template<class value_type, class comporator = std::less<value_type>>
class CRBTree{
	
#include "CRBTreeNode.hpp"

#include "CRBTreeStdIterator.hpp"

#include "CRBTraversalIterators.hpp"

#include "StdTreeFunctions.hpp"

#include "CRBTreeModifyFunctions.hpp"

public:

	CRBTree() { _root = nullptr; _size = 0; }
	CRBTree(const CRBTree& tree) { _root = copyTree(tree._root, nullptr); _size = tree.size(); }
	~CRBTree() { freeMemOfTree(_root); }
	

	const_iterator insert(const value_type& value) {
		CRBNode* new_node;
		_root = insert_node(value, &new_node);
		return const_iterator(_root, new_node);
	}
	
	void remove(const value_type& value){
		CRBNode* n = findNode(value);
		if(n != nullptr) {
			--_size;
			_root = remove_node_by_pointer(_root, n);
		}
	}
	
	const_iterator find(const value_type& value) const {
		return const_iterator(_root, findNode(value));
	}

	

	const_iterator begin() const { return const_iterator(_root, findMin(_root)); }
	
	iteratorPreorderDF beginPreorderDF() const { return iteratorPreorderDF(_root); }
	iteratorInorderDF beginInorderDF() const { return iteratorInorderDF(_root); }
	iteratorPostorderDF beginPostorderDF() const { return iteratorPostorderDF(_root); }
	iteratorBF beginBF() const { return iteratorBF(_root); }
	
	// END iterator is usable for ALL times of iterators
	// (both usual and traversal iterators)
	const_iterator end() const { return const_iterator(_root, nullptr, false, true); }
	
	
	
	
	
	void clear() {
		freeMemOfTree(_root);
		_root = nullptr;
		_size = 0;
	}
	
	bool empty() const { return (_size == 0); }
	size_t size() const { return _size; }
	
protected:
	CRootRBNode* _root;
	size_t _size;
};

#endif /* CRBTree_h */





















