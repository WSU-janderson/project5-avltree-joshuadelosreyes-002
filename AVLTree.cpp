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
	switch (current->numChildren()) {
		/** Case 1: We can delete the node. */ 
		case 0: {
			current = nullptr;
			break;
		}

		/** Case 2: Replace current with its only child. */
		case 1: {
			if (current->right) {
				current = current->right;
			} else {
				current = current->left;
			}
			break;
		}

		/** Case 3: We have two children.
		 *	Get smallest key in right subtree by
		 *	getting right child and go left until left is null. */
		case 2: {
			AVLNode *minRight = current->right;

			/** I could check if smallestInRight is null,
			 *	but it shouldn't be since the node has two children. */
			while (minRight->left) {
				minRight = minRight->left;
			}

			KeyType newKey = minRight->key;
			ValueType newValue = minRight->value;
			bool nodeRemoved = this->remove(this->root, newKey); // Delete this one.

			current->key = newKey;
			current->value = newValue;

			current->height = current->getHeight();
			this->balanceNode(current);

			return nodeRemoved;
		}
	}

	delete toDelete;
	return true;
}

bool AVLTree::remove(const KeyType &key) {
	bool nodeRemoved = this->remove(this->root, key);
	if (nodeRemoved) {--this->length;}
	return nodeRemoved;
}

bool AVLTree::remove(AVLNode *&current, KeyType key) {
	if (current) {
		if (current->key < key) {
			return this->remove(current->left, key);
		} else if (current->key > key) {
			return this->remove(current->right, key);
		} else {
			return this->removeNode(current);
		}
	} else {
		return false;
	}
}

void AVLTree::balanceNode(AVLNode *&node) {

}

/** Creates an empty AVL tree. */
AVLTree::AVLTree() : root(nullptr), length(0) {}

/**
 *	Creates a node, loaded with a key-value pair.
 *	New nodes are allocated as leaf nodes, which they have no children.
 */
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
bool AVLTree::contains(const AVLNode *current, const KeyType &key) const {
	if (current) {
		if (current->key < key) {
			return this->contains(current->left, key);
		} else if (current->key > key) {
			return this->contains(current->right, key);
		} else {
			return true;
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

/**
 *	Returns the height of the tree.
 *	If the tree is empty, its height is `-1`.
 */
size_t AVLTree::getHeight() const {
	if (this->root) {
		return this->root->height;
	} else {
		return -1;
	}
}

/**
 *	Recursive helper to traverse the nodes of the tree at the proper depth.
 *	This uses the right child first in-order traversal in the tree.
 */
void AVLTree::printDepth(std::ostream &os, const AVLNode *node, const size_t depth) {
	if (node) {
		AVLTree::printDepth(os, node->right, depth + 1);
		os << std::string(2 * depth, ' ') << node << "\n";
		AVLTree::printDepth(os, node->left, depth + 1);
	}
	return;
}

/**
 *	Prints an individual node represented by `{<key>: <value>}`.
 */
std::ostream & operator<<(std::ostream &os, const AVLTree::AVLNode *node) {
	os << "{" << node->key << ": " << node->value << "}";
	return os;
}

/**
 *	Prints every node in the tree that resembles the tree's structure.
 */
std::ostream & operator<<(std::ostream &os, const AVLTree &avlTree) {
	AVLTree::printDepth(os, avlTree.root, 0);
	return os;
}

/**
 *	Returns the value associated with the specified `key` if it exists.
 *	Otherwise, if that key doesn't exist in the tree, `std::nullopt` is returned.
 *
 *	Expected time complexity is `O(log(n))`.
 */
std::optional<AVLTree::ValueType> AVLTree::get(const KeyType &key) const {
	return this->get(this->root, key);
}

/**
 *	Recursive helper to traverse the nodes of the tree to get to the matching key,
 *	similar to `contains()`.
 */
std::optional<AVLTree::ValueType> AVLTree::get(const AVLNode *current, const KeyType &key) const {
	if (current) {
		if (current->key < key) {
			return this->get(current->left, key);
		} else if (current->key > key) {
			return this->get(current->right, key);
		} else {
			return std::optional{current->value};
		}
	} else {
		return std::nullopt;
	}
}

/**
 *	Returns the value associated with the specified `key`.
 *	But the value of that key can also be updated.
 *
 *	This method may be ill-formed if a key does not exist.
 *
 *	Expected time complexity is `O(log(n))`.
 */
AVLTree::ValueType & AVLTree::operator[](const KeyType &key) {
	return this->getValue(this->root, key);
}

/**
 *	Recursive helper to traverse the nodes of the tree to get a reference holding
 *	the value corresponding to a matching `key`.
 *
 *	A missing key caused by a null node not returning anything may cause undesired behavior.
 */
AVLTree::ValueType & AVLTree::getValue(AVLNode *current, const KeyType &key) {
	if (current) {
		if (current->key < key) {
			return this->getValue(current->left, key);
		} else if (current->key > key) {
			return this->getValue(current->right, key);
		} else {
			ValueType &valueOf = current->value;
			return valueOf;
		}
	}
}

/**
 *	Returns a vector containing all the keys in the tree.
 *	The vector has the same size as the tree's size.
 */
std::vector<AVLTree::KeyType> AVLTree::keys() const {
	std::vector<KeyType> keyList;
	this->grabKey(keyList, this->root);
	return keyList;
}

/**
 *	Recursive helper to traverse the nodes of the tree to grab a key from a node
 *	and insert that key in a vector called the key list.
 */
void AVLTree::grabKey(std::vector<AVLTree::KeyType> &keyList, const AVLNode *current) const {
	if (current) {
		this->grabKey(keyList, current->left);
		keyList.push_back(current->key);
		this->grabKey(keyList, current->right);
	}
	return;
}

/**
 *	Returns a vector of all unique values that correspond to the keys that are in a range
 *	bounded by `low` and `high`.
 */
std::vector<AVLTree::ValueType> AVLTree::findRange(const KeyType &low, const KeyType &high) const {
	std::vector<ValueType> valueList;
	this->grabValue(valueList, this->root, low, high);
	return valueList;
}

/**
 *	Because the values in the list is not expected to be sorted, searching for an element in a vector
 *	is processed linearly. If such value exists, it won't be inserted in that vector.
 */
void insertValue(std::vector<AVLTree::ValueType> &valueList, const AVLTree::ValueType &value) {
	bool isUnique = true;
	for (size_t i = 0; (i < valueList.size()) && isUnique; ++i) {
		isUnique &= valueList[i] != value;
	}
	if (isUnique) {valueList.push_back(value);}
}

/**
 *	Recursive helper to traverse the nodes of the tree to grab a value from a node
 *	and insert that value in a vector if it doesn't exist, given the key is in the
 *	specified bounds.
 *
 *	Check if such child exists in order to compare that key to the bounds.
 */
void AVLTree::grabValue(
	std::vector<AVLTree::ValueType> &valueList, const AVLNode *current,
	const KeyType &low, const KeyType &high
) const {
	if (current) {
		if (current->left) {
			if (current->left->key >= low) {
				this->grabValue(valueList, current->left, low, high);
			}
		}
		insertValue(valueList, current->value);
		if (current->right) {
			if (current->right->key <= high) {
				this->grabValue(valueList, current->right, low, high);
			}
		}
	}
	return;
}
