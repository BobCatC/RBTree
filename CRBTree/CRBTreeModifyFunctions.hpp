#ifndef CRBTreeModifyFunctions_h
#define CRBTreeModifyFunctions_h

/* 	 Always is included into CRBTree.h
 *	( inside class CRBTree<type_name, comporator> )
 */

private:

	CRootRBNode* insert_node(const value_type& value, CRBNode** new_node_return) {
		CRootRBNode* root = _root;
		CRBNode* crt = root;
		CRBNode* new_node = nullptr;
		if(root == nullptr){
			root = new CRootRBNode(value);
			new_node = root;
			++_size;
		}
		else{
			while(true){
				if((*crt) < value){
					if(crt->hasRightChild()){
						crt = crt->getRightChild();
					}
					else{
						crt->setRightChild(new_node = new CRBNode(value, crt));
						++_size;
						break;
					}
				}
				else{
					if((*crt) != value){
						if(crt->hasLeftChild()){
							crt = crt->getLeftChild();
						}
						else{
							crt->setLeftChild(new_node = new CRBNode(value, crt));
							++_size;
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
		*new_node_return = new_node == nullptr ? crt : new_node;
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




	CRootRBNode* remove_node_by_pointer(CRootRBNode* root, CRBNode* n) {
	
		if(n->hasLeftChild() && n->hasRightChild()){
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


#endif /* CRBTreeModifyFunctions_h */













