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

AVLTree::AVLTree() : root(nullptr), length(0) {}

AVLTree::AVLNode::AVLNode(const KeyType &key, const ValueType &value) : 
	key(key), value(value),
	left(nullptr), right(nullptr), height(0) {}

/**
 *	Returns the number of existing key-value pairs in the tree.
 *	The `size` only updates if a successful insertion or removal of a pair had occurred.
 *
 *	Expected time complexity is `O(1)`.
 */
size_t AVLTree::size() const {return this->length;}

/**
 *	Returns `true` if and only if the specified `key` is in the tree.
 *
 *	Expected time complexity is `O(log(n))`.
 */
bool AVLTree::contains(const KeyType &key) const {
	return this->contains(this->root, key);
}

/**
 *	Recursive helper method in the perspective of a node.
 *	It should return `false` once the node becomes `null`.
 */
bool AVLTree::contains(AVLNode *current, const KeyType &key) const {
	if (current) {
		if (current->key == key) {return true;}
		else {
			if (current->key < key) {
				return this->contains(current->left, key);
			} else {
				return this->contains(current->right, key);
			}
		}
	} else {
		return false;
	}
}

/**
 *	Insert a new key-value pair into the tree.
 *	If a key was not in the tree, the new key is uniquely inserted, so that
 *	should return `true`. Otherwise, it returns `false` even though a new value
 *	will be replacing the old value corresponding to an existing key.
 *
 *	Expected time complexity is `O(log(n))`.
 */
bool AVLTree::insert(const KeyType &key, ValueType value) {
	bool uniqueInsert = this->insert(this->root, key, value);
	if (uniqueInsert) {++this->length;}
	return uniqueInsert;
}

/**
 *	Recursive helper method in the perspective of a node.
 *
 *	An insertion usually allocates a new node on one of the child pointers and
 *	doesn't go into that child, thus there is no way for another recursive call
 *	with a null node as an argument.
 *
 *	On top of insertion, after a successful insert, all of its parent nodes
 *	should recalculate their heights and consider their height balances.
 *	Any necessary rotations occur somewhere in this recursive form.
 */
bool AVLTree::insert(AVLNode *current, const KeyType &key, ValueType &value) {
	if (current) {
		if (current->key == key) {
			current->value = value;
			return false;
		} else {
			bool uniqueInsert;

			if (current->key < key) {
				if (current->left) {
					uniqueInsert = this->insert(current->left, key, value);
				} else {
					current->left = new AVLNode(key, value);
					uniqueInsert = true;
				}
			} else {
				if (current->right) {
					uniqueInsert = this->insert(current->right, key, value);
				} else {
					current->right = new AVLNode(key, value);
					uniqueInsert = true;
				}
			}

			if (uniqueInsert) {
				size_t lh = current->left ? current->left->height : -1;
				size_t rh = current->right ? current->right->height : -1;
				current->height = max(lh + 1, rh + 1);


			}

			return uniqueInsert;
		}
	} else {
		if (current == this->root) {
			AVLNode *node = new AVLNode(key, value);
			this->root = node;
			return true;
		}
	}
}
