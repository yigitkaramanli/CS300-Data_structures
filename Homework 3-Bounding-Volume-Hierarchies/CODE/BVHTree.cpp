#include "BVHTree.h"

void BVHTree::printNode(std::ostream &out, BVHTreeNode *node, int level) {
	if (root == nullptr) return;
	for (int i = 0; i < level; i++) {
		out << "  ";
	}
	if (!node->isLeaf) {
		out << "+ branch || ";
		node->aabb.printAABB(out);
		out << std::endl;
		printNode(out, node->rightChild, level + 1);
		printNode(out, node->leftChild, level + 1);
	}
	else {
		out << "- ";
		if (node->parent) {
			if (node->parent->rightChild == node)
				out << "R ";
			else
				out << "L ";
		}
		out << "- leaf: " << node->name << " || ";
		node->aabb.printAABB(out);
		out << std::endl;
	}
}
std::ostream &operator<<(std::ostream &out, BVHTree &tree) {
	tree.printNode(out, tree.root, 0);
	return out;
}
BVHTree::BVHTree() {
	root = NULL;
}
BVHTree::~BVHTree() {
	recursivedelete(root);
}
void BVHTree::recursivedelete(BVHTreeNode* node) {
	if (node != NULL) {
		recursivedelete(node->leftChild);
		recursivedelete(node->rightChild);

		map.erase(node->name);
		delete node;
		node = NULL;
	}
}

void BVHTree::addBVHMember(AABB objectArea, std::string name) {
	BVHTreeNode* newnode = new BVHTreeNode(objectArea, name, true);
	if (root == NULL) {              // If the tree is empty
		root = newnode;
		map[name] = newnode;
	}
	else if (root->isLeaf) {        //There is exactly one node in the tree
		AABB brancharea = root->aabb + newnode->aabb;
		BVHTreeNode* newbranch = new BVHTreeNode(brancharea,"branch",false);
		newbranch->rightChild = root;
		root->parent = newbranch;
		newbranch->leftChild = newnode;
		newnode->parent = newbranch;
		root = newbranch;
		map[name] = newnode;
	}
	else {                             // There is more than one node in the tree
		bool right;
		BVHTreeNode* dummy = root;
		BVHTreeNode* tobeins;
		while (!dummy->isLeaf) {
			if (inceraseInRightTreeSize(newnode, dummy) < increaseInLeftTreeSize(newnode, dummy)) {
				dummy = dummy->rightChild;
				right = true;
			}
			else {
				dummy = dummy->leftChild;
				right = false;
			}
		}
		AABB brancharea = dummy->aabb + newnode->aabb;
		BVHTreeNode* newbranch = new BVHTreeNode(brancharea, "branch", false);
		newbranch->leftChild = newnode;
		newnode->parent = newbranch;
		newbranch->rightChild = dummy;
		newbranch->parent = dummy->parent;
		if (right) {
			dummy->parent->rightChild = newbranch;
		}
		else {
			dummy->parent->leftChild = newbranch;
		}
		dummy->parent = newbranch;
		map[name] = newnode;
		dummy = newbranch;
		while (dummy->parent != NULL) {
			dummy = dummy->parent;
			dummy->aabb = dummy->rightChild->aabb + dummy->leftChild->aabb;
		}
	}
}
void BVHTree::moveBVHMember(std::string name, AABB newLocation) {
	if (map.find(name) != map.end()) {
		BVHTreeNode* tomove = map[name];
		if (tomove == root)                           //1 
			tomove->aabb = newLocation;					//2 
		else{										//3
			if (tomove->parent->aabb.getArea() == AABB::unionArea(tomove->parent->aabb, newLocation)) {
				tomove->aabb = newLocation;
			}
			else {
				removeBVHMember(name);
				addBVHMember(newLocation, name);
			}
		}                              //4
	}
}

void BVHTree::removeBVHMember(std::string name) {
	if (map.find(name) != map.end()) {
		BVHTreeNode* toremove = map[name];
		map.erase(name);
		BVHTreeNode* removedbranch = toremove->parent;
		BVHTreeNode* dummy = removedbranch->parent;
		bool right;
		if (removedbranch == NULL ) {
			delete toremove;
			root = NULL;
			return;
		}
		else if(removedbranch->parent==NULL){
			if (toremove == removedbranch->rightChild) {
				root = removedbranch->leftChild;
				delete toremove;
				toremove = NULL;
				delete removedbranch;
				removedbranch = NULL;
			}
			else {
				root = removedbranch->rightChild;
				delete toremove;
				toremove = NULL;
				delete removedbranch;
				removedbranch = NULL;
			}
		}
		else if(removedbranch->rightChild == toremove){
			removedbranch->leftChild->parent = removedbranch->parent;
			if (isgrannysright(removedbranch)) {
				removedbranch->parent->rightChild = removedbranch->leftChild;
			}
			else {
				removedbranch->parent->leftChild = removedbranch->leftChild;
			}
			delete toremove;
			delete removedbranch;
		}
		else {
			removedbranch->rightChild->parent = removedbranch->parent;
			if (isgrannysright(removedbranch)) {
				removedbranch->parent->rightChild = removedbranch->rightChild;
			}
			else {
				removedbranch->parent->leftChild = removedbranch->rightChild;
			}

			delete toremove;
			delete removedbranch;
		}
		while ( dummy != NULL && !dummy->isLeaf) {
			dummy->aabb = dummy->rightChild->aabb + dummy->leftChild->aabb;
			dummy = dummy->parent;
		}
	}
}
bool BVHTree::isgrannysright(BVHTreeNode * node) {
	return (node->parent->rightChild == node);
}

std::vector<std::string> BVHTree::getCollidingObjects(AABB object) {
	std::vector<std::string> vector;
	collidededector(vector, object, root);
	return vector;
}
void BVHTree::collidededector(std::vector<std::string>& vec, AABB object,BVHTreeNode * node) {
	if (node->aabb.collide(object)) {
		if (!node->isLeaf) {
			collidededector(vec,object,node->leftChild);
			collidededector(vec, object, node->rightChild);
		}
		else {
			vec.push_back(node->name);
		}
	}
}

int BVHTree::inceraseInRightTreeSize(BVHTreeNode* newNode, BVHTreeNode* branchNode) {
	return AABB::unionArea(newNode->aabb,
		branchNode->rightChild->aabb) - branchNode->rightChild->aabb.getArea();
}

int BVHTree::increaseInLeftTreeSize(BVHTreeNode* newNode, BVHTreeNode* branchNode) {
	return AABB::unionArea(newNode->aabb,
		branchNode->leftChild->aabb) - branchNode->leftChild->aabb.getArea();
}