#include "AVLTree.h"

size_t AVLTree::AVLNode::numChildren() const {
	size_t count = 0;
	if (this->left) {++count;}
	if (this->right) {++count;}
	return count;
}

bool AVLTree::AVLNode::isLeaf() const {
	return this->numChildren() == 0;
}

size_t max(size_t a, size_t b) {
	return (a < b) ? b : a;
}

size_t AVLTree::AVLNode::getHeight() const {
	size_t lh = this->left ? this->left->height : -1;
	size_t rh = this->right ? this->right->height : -1;
	return max(lh + 1, rh + 1);
}

bool AVLTree::removeNode(AVLNode *&current) {
	if (!current) {
		return false;
	}

	AVLNode *toDelete = current;
	size_t nChildren = current->numChildren();
	if (current->isLeaf()) {
		/** Case 1: We can delete the node. */ 
		current = nullptr;
	} else if (current->numChildren() == 1) {
		/** Case 2: Replace current with its only child. */
		if (current->right) {
			current = current->right;
		} else {
			current = current->left;
		}
	} else {
		/** Case 3: We have two children.
		 *	Get smallest key in right subtree by
		 *	getting right child and go left until left is null. */
		AVLNode *smallestInRight = current->right;

		/** I could check if smallestInRight is null,
		 *	but it shouldn't be since the node has two children. */
		while (smallestInRight->left) {
			smallestInRight = smallestInRight->left;
		}

		std::string newKey = smallestInRight->key;
		int newValue = smallestInRight->value;
		remove(root, smallestInRight->key); // Delete this one.

		current->key = newKey;
		current->value = newValue;

		current->height = current->getHeight();
		balanceNode(current);

		return true; // We already deleted the one we needed to so return.
	}

	delete toDelete;

	return true;
}

bool AVLTree::remove(AVLNode *&current, KeyType key) {
	return false;
}

void AVLTree::balanceNode(AVLNode *&node) {

}


