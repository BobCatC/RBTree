//
//  StdTreeFunctions.hpp
//  RBTree
//
//  Created by Александр Пахомов on 22.05.2018.
//  Copyright © 2018 Александр Пахомов. All rights reserved.
//

#ifndef StdTreeFunctions_h
#define StdTreeFunctions_h


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



static void freeMemOfTree(CRBNode* crt){
	CRBNode* left, *right;
	if(crt == nullptr)
		return;
	left = crt->getLeftChild();
	right = crt->getRightChild();
	delete crt;
	freeMemOfTree(left);
	freeMemOfTree(right);
}

static CRBNode* copyTree(CRBNode* crtOther, CRBNode* parent) {
	CRBNode* crt = nullptr;
	if(crtOther != nullptr){
		crt = new CRBNode(crtOther->getValue(), parent, nullptr, nullptr, crt->isBlack() ? black : red);
		crt->setLeftChild(copyTree(crtOther->getLeftChild(), crt));
		crt->setRightChild(copyTree(crtOther->getRightChild(), crt));
	}
	return crt;
}

#endif /* StdTreeFunctions_h */
