/**
 *	AVLTree.cpp
 */

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

ssize_t AVLTree::AVLNode::getBalance() const {
	ssize_t lh = this->left ? this->left->height : -1;
	ssize_t rh = this->right ? this->right->height : -1;
	return lh - rh;
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

			/**	The height of this node may be properly updated when returning back
			 *	from rebalancing after deleting the other node. */
			current->key = newKey;
			current->value = newValue;

			return nodeRemoved;
		}
	}

	delete toDelete;
	return true;
}

bool AVLTree::remove(const KeyType &key) {
	bool nodeRemoved = this->remove(this->root, key);
	if (nodeRemoved) {
		--this->length;
		this->balanceNode(nullptr, Direction::NONE);
	}
	return nodeRemoved;
}

bool AVLTree::remove(AVLNode *&current, KeyType key) {
	if (current) {
		bool nodeRemoved;
		Direction whichChild = Direction::NONE;

		if (key < current->key) {
			nodeRemoved = this->remove(current->left, key);
			whichChild = Direction::LEFT;
		} else if (key > current->key) {
			nodeRemoved = this->remove(current->right, key);
			whichChild = Direction::RIGHT;
		} else {
			return this->removeNode(current);
		}

		if (nodeRemoved) {this->balanceNode(current, whichChild);}
		return nodeRemoved;
	} else {
		return false;
	}
}

/**
 *	Check the height balances of this `node`'s children, specified by direction.
 *	If `node` is `nullptr`, this means the balancing will act on the root node.
 *	The direction of the node's child should not be `NONE`.
 *
 *	Only one child can have a height balance whose absolute value is
 *	greater than 1. If so, this node is the parent node of one of the
 *	children whose height is greatly unbalanced.
 */
void AVLTree::balanceNode(AVLNode *node, const AVLTree::Direction &childDir) {
	if (node) {node->height = node->getHeight();}
	AVLNode *child = node ? (
		(childDir == Direction::LEFT) ? node->left :
		(childDir == Direction::RIGHT) ? node->right : (
			!(node->left) ? node->right :
			!(node->right) ? node->left : nullptr
		)
	) : this->root;

	if (child) {
		AVLNode *grandChild = nullptr;
		size_t rotations = 0;

		if (child->getBalance() > Direction::LEFT) {
			++rotations;
			grandChild = child->left;
			if (grandChild->getBalance() < Direction::LEFT) {
				++rotations;
				child->rotateLeft(Direction::LEFT);
			} if (node) {
				node->rotateRight(Direction::LEFT);
			} else {
				this->rotateRight();
			}
		} else if (child->getBalance() < Direction::RIGHT) {
			++rotations;
			grandChild = child->right;
			if (grandChild->getBalance() > Direction::RIGHT) {
				++rotations;
				child->rotateRight(Direction::RIGHT);
			} if (node) {
				node->rotateLeft(Direction::RIGHT);
			} else {this->rotateLeft();}
		}

		/**
		 *	Other child nodes that are carried by nodes affected by rotations do not
		 *	need to change heights.
		 *
		 *	However, if rotations occur, after these rotations, height adjustments
		 *	occur at the deepest affected nodes first.
		 */
		switch (rotations) {
			case 0: {
				if (grandChild) {grandChild->height = grandChild->getHeight();}
				child->height = child->getHeight();
				if (node) {node->height = node->getHeight();}
				break;
			} case 1: {
				if (node) {node->height = node->getHeight();}
				if (grandChild) {grandChild->height = grandChild->getHeight();}
				child->height = child->getHeight();
				break;
			} case 2: {
				if (node) {node->height = node->getHeight();}
				child->height = child->getHeight();
				if (grandChild) {grandChild->height = grandChild->getHeight();}
				break;
			}
		}
	}

	return;
}

/**
 *	`this` node is the parent node.
 *	One of this node's children specified by a direction
 *	is rotated to the left.
 */
void AVLTree::AVLNode::rotateLeft(const AVLTree::Direction &childDir) {
	AVLNode *temp;
	switch (childDir) {
		case Direction::LEFT: {
			temp = this->left;
			this->left = temp->right;
			temp->right = this->left->left;
			this->left->left = temp;
			return;
		} case Direction::RIGHT: {
			temp = this->right;
			this->right = temp->right;
			temp->right = this->right->left;
			this->right->left = temp;
			return;
		} default: {return;}
	}
}

/**
 *	Rotate the root node to the left.
 */
void AVLTree::rotateLeft() {
	AVLNode *temp = this->root;
	this->root = temp->right;
	temp->right = this->root->left;
	this->root->left = temp;
}

/**
 *	`this` node is the parent node.
 *	One of this node's children specified by a direction
 *	is rotated to the right.
 */
void AVLTree::AVLNode::rotateRight(const AVLTree::Direction &childDir) {
	AVLNode *temp;
	switch (childDir) {
		case Direction::LEFT: {
			temp = this->left;
			this->left = temp->left;
			temp->left = this->left->right;
			this->left->right = temp;
			return;
		} case Direction::RIGHT: {
			temp = this->right;
			this->right = temp->left;
			temp->left = this->right->right;
			this->right->right = temp;
			return;
		} default: {return;}
	}
}

/**
 *	Rotate the root node to the right.
 */
void AVLTree::rotateRight() {
	AVLNode *temp = this->root;
	this->root = temp->left;
	temp->left = this->root->right;
	this->root->right = temp;
}

/** Creates an empty AVL tree. */
AVLTree::AVLTree() : root(nullptr), length(0) {}

/**
 *	Create a copy of another AVL tree.
 *
 *	Because a tree has pointers to other nodes, given each node must be
 *	independent from each other, copying another AVL tree requires a deep
 *	copy.
 *
 *	This copy traverses all the nodes in the `other` tree using pre-order
 *	traversal, and creates new nodes based on the key-value pairs of the
 *	`other` tree.
 */
AVLTree::AVLTree(const AVLTree &other) {
	this->length = other.length;
	this->insert(this->root, other.root);
}

/**
 *	Upon deletion, removes all nodes of the AVL tree.
 *
 *	The removal of all nodes uses post-order traversal so that all child
 *	nodes are deleted until the target node.
 */
AVLTree::~AVLTree() {
	this->remove(this->root);
}

/**
 *	Recursive helper method to traverse the other tree's nodes to make
 *	copies of these nodes that are independent of each other.
 */
void AVLTree::insert(AVLNode *&current, const AVLNode *other) {
	if (other) {
		current = new AVLNode(other->key, other->value);
		this->insert(current->left, other->left);
		this->insert(current->right, other->right);
		current->height = current->getHeight();
	}
	return;
}

/**
 *	Recursive helper method to traverse the nodes of the AVL tree
 *	by deleting all the node's children before the current node.
 */
void AVLTree::remove(AVLNode *&current) {
	if (current) {
		this->remove(current->left);
		this->remove(current->right);
		delete current;
	}
	current = nullptr;
	return;
}

/**
 *	Assign a copy of the `other` AVL tree to this AVL tree.
 *	Before deep copying the `other` tree's nodes, all the current nodes
 *	must be removed.
 */
void AVLTree::operator=(const AVLTree &other) {
	this->remove(this->root);
	this->length = other.length;
	this->insert(this->root, other.root);
}

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
		if (key < current->key) {
			return this->contains(current->left, key);
		} else if (key > current->key) {
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
	if (uniqueInsert) {
		++this->length;
		this->balanceNode(nullptr, Direction::NONE);
	}
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
		if (key == current->key) {
			current->value = value;
			return false;
		} else {
			bool uniqueInsert;
			Direction whichChild = Direction::NONE;

			if (key < current->key) {
				if (current->left) {
					uniqueInsert = this->insert(current->left, key, value);
					whichChild = Direction::LEFT;
				} else {
					current->left = new AVLNode(key, value);
					uniqueInsert = true;
				}
			} else {
				if (current->right) {
					uniqueInsert = this->insert(current->right, key, value);
					whichChild = Direction::RIGHT;
				} else {
					current->right = new AVLNode(key, value);
					uniqueInsert = true;
				}
			}

			if (uniqueInsert) {this->balanceNode(current, whichChild);}
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
		if (key < current->key) {
			return this->get(current->left, key);
		} else if (key > current->key) {
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
		if (key < current->key) {
			return this->getValue(current->left, key);
		} else if (key > current->key) {
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
			this->grabValue(valueList, current->left, low, high);
		} if ((low <= current->key) && (current->key <= high)) {
			insertValue(valueList, current->value);
		} if (current->right) {
			this->grabValue(valueList, current->right, low, high);
		}
	}
	return;
}
