#ifndef CBsTTraversalIterators_h
#define CBsTTraversalIterators_h

/* 	 Always is included into CBsT.hpp
 *	( inside class CBsT<type_name, comporator> )
 */


private:
#define crt_node iterator_traversal::_crt

class iterator_traversal{
protected:
	CRBNode* _crt;
	bool _fIsAfterLastNode;
	virtual void findNext() { }
	typedef iterator_traversal self_type;
	
	bool next() {
		if(_crt != nullptr){
			findNext();
		}
		return ( _crt != nullptr );
	}
	
	void tryNext() {
		if(!_fIsAfterLastNode){
			if(!next()){
				_fIsAfterLastNode = true;
			}
		}
	}
	
public:
	iterator_traversal(CRBNode* root) :
	_crt(root),
	_fIsAfterLastNode(root == nullptr) {}
	
	const value_type& operator*() { return _crt->getValue(); }
	
	/* 		For comparing widt CBsT.end() iterator		*/
	bool operator==(const const_iterator& it) { return ( _fIsAfterLastNode && it.fIsEndIterator() ); }
	bool operator!=(const const_iterator& it) { return !( *this == it ); }
	
	/* 		For comparing with other heirs of class "iterator_traversal"		*/
	bool operator==(const iterator_traversal& it) { return ( (_fIsAfterLastNode && it._fIsAfterLastNode) || *_crt == *it._crt ); }
	bool operator!=(const iterator_traversal& it) { return !( *this == it ); }
};

public:
class iteratorPreorderDF : public iterator_traversal{
private:
	enum EInstruction{
		go_down_left,
		go_once_down_right,
		go_up
	}instruction;
	
	void findNext() override {
		switch (instruction) {
			case go_down_left:
				if(crt_node->fHasLeftChild()){
					crt_node = crt_node->getLeftChild();
				}
				else{
					instruction = go_once_down_right;
					findNext();
				}
				break;
			case go_once_down_right:
				if(crt_node->fHasRightChild()){
					crt_node = crt_node->getRightChild();
					instruction = go_down_left;
				}
				else{
					instruction = go_up;
					findNext();
				}
				break;
			case go_up:
				if(!crt_node->isRoot()){
					if(crt_node->isLeftChild()){
						instruction = go_once_down_right;
						crt_node = crt_node->getParent();
						findNext();
					}
					else{
						instruction = go_up;
						crt_node = crt_node->getParent();
						findNext();
					}
				}
				else{
					crt_node = nullptr;
				}
				break;
		}
	}
public:
	typedef iteratorPreorderDF self_type;
	iteratorPreorderDF(CRBNode* root) : iterator_traversal(root) {
		instruction = go_down_left;
	}
	
	self_type operator++() {
		iterator_traversal::tryNext();
		return *this;
	}
	self_type operator++(int junk) {
		self_type it = *this;
		iterator_traversal::tryNext();
		return it;
	}
};

class iteratorPostorderDF : public iterator_traversal {
private:
	enum EInstruction{
		go_down_left,
		go_once_down_right,
		go_up
	}instruction;
	
	void findNext() override {
		switch (instruction) {
			case go_down_left:
				while(crt_node->fHasLeftChild()){
					crt_node = crt_node->getLeftChild();
				}
				instruction = go_once_down_right;
				findNext();
				break;
			case go_once_down_right:
				if(crt_node->fHasRightChild()){
					crt_node = crt_node->getRightChild();
					instruction = go_down_left;
					findNext();
				}
				else{
					instruction = go_up;
					return;
				}
				break;
			case go_up:
				if(crt_node->getParent() != nullptr){
					if(crt_node->isLeftChild()){
						instruction = go_once_down_right;
						crt_node = crt_node->getParent();
						findNext();
					}
					else{
						instruction = go_up;
						crt_node = crt_node->getParent();
						return;
					}
				}
				else{
					crt_node = nullptr;
				}
				break;
		}
	}
public:
	typedef iteratorPostorderDF self_type;
	iteratorPostorderDF(CRBNode* root) : iterator_traversal(root) {
		instruction = go_down_left;
		if(!iterator_traversal::_fIsAfterLastNode)
			findNext();
	}
	self_type operator++() {
		iterator_traversal::tryNext();
		return *this;
	}
	self_type operator++(int junk) {
		self_type it = *this;
		iterator_traversal::tryNext();
		return it;
	}
};

class iteratorInorderDF : public iterator_traversal {
private:
	enum EInstruction{
		go_down_left,
		go_once_down_right,
		go_up
	}instruction;
	
	void findNext() override {
		switch (instruction) {
			case go_down_left:
				while(crt_node->fHasLeftChild()){
					crt_node = crt_node->getLeftChild();
				}
				instruction = go_once_down_right;
				break;
			case go_once_down_right:
				if(crt_node->fHasRightChild()){
					crt_node = crt_node->getRightChild();
					instruction = go_down_left;
					findNext();
				}
				else{
					instruction = go_up;
					findNext();
				}
				break;
			case go_up:
				if(crt_node->getParent() != nullptr){
					if(crt_node->isLeftChild()){
						instruction = go_once_down_right;
						crt_node = crt_node->getParent();
						return;
					}
					else{
						instruction = go_up;
						crt_node = crt_node->getParent();
						findNext();
						return;
					}
				}
				else{
					crt_node = nullptr;
				}
				
				break;
		}
	}
public:
	typedef iteratorInorderDF self_type;
	iteratorInorderDF(CRBNode* root) : iterator_traversal(root) {
		instruction = go_down_left;
		if(!iterator_traversal::_fIsAfterLastNode)
			findNext();
	}
	
	self_type operator++() {
		iterator_traversal::tryNext();
		return *this;
	}
	self_type operator++(int junk) {
		self_type it = *this;
		iterator_traversal::tryNext();
		return it;
	}
};

class iteratorBF : public iterator_traversal {
private:
	std::queue<CRBNode*> firstStack, secondStack;
	std::queue<CRBNode*>& inputStack;
	std::queue<CRBNode*>& outputStack;
	
	void findNext() override {
		if(inputStack.size() != 0){
			crt_node = inputStack.front();
			
			inputStack.pop();
			if(crt_node->fHasLeftChild()){
				outputStack.push(crt_node->getLeftChild());
			}
			if(crt_node->fHasRightChild()){
				outputStack.push(crt_node->getRightChild());
			}
		}
		else{
			if(outputStack.size() != 0){
				const std::queue<CRBNode*>& c = inputStack;
				inputStack = outputStack;
				outputStack = c;
				while(outputStack.size() != 0){
					outputStack.pop();
				}
				findNext();
			}
			else{
				crt_node = nullptr;
			}
		}
	}
public:
	typedef iteratorBF self_type;
	
	iteratorBF(CRBNode* root) :
	inputStack(firstStack),
	outputStack(secondStack),
	iterator_traversal(root) {
		inputStack.push(root);
		if(!iterator_traversal::_fIsAfterLastNode)
			findNext();
	}
	
	self_type operator++() {
		iterator_traversal::tryNext();
		return *this;
	}
	self_type operator++(int junk) {
		self_type it = *this;
		iterator_traversal::tryNext();
		return it;
	}
};


#endif /* CBsTTraversalIterators_h */
