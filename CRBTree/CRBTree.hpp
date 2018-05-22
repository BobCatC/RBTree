#ifndef CRBTree_h
#define CRBTree_h

#include <iostream>
#include <queue>
using namespace std;


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
	
	
	
	
	
	const CRootRBNode* getRoot() const { return _root; }
	
private:
	
	
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
	
#include "CRBTreeModifyFunctions.hpp"
	
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
		
		CRBNode* child = n->getLeftChild()->isNullLeaf() ? n->getRightChild() : n->getLeftChild();
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
	
#include "StdTreeFunctions.hpp"
	
	
	
	
protected:
	CRootRBNode* _root;
	int crtBlackHeight;
};

#endif /* CRBTree_h */





















