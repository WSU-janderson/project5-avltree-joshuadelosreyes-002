/**
 *	AVLTree.h
 */

#ifndef AVLTREE_H
#define AVLTREE_H

#include <string>
#include <optional>
#include <vector>
#include <ostream>

class AVLTree {
	public:
		using KeyType = std::string;
		using ValueType = size_t;

	protected:

		/**
		 *	Rebalancing nodes, in particular, rotations, may require a parent node
		 *	to change one of the child node pointers. This occurs if a successful
		 *	insertion and deletion occured and the absolute value of the height
		 *	balance is greater than `1`.
		 */
		enum class Direction : ssize_t {

			/**
			 *	A successful insertion or deletion occurred on the left child;
			 *	or a node is unbalanced to the left.
			 */
			LEFT = 1,

			/**
			 *	This resultant child direction only exists at the root node pointer;
			 *	or a node has equal heights of each child.
			 */
			NONE = 0,

			/**
			 *	A successful insertion or deletion occurred on the right child;
			 *	or a node is unbalanced to the right.
			 */
			RIGHT = -1,
		};

		class AVLNode {
			public:
				KeyType key;
				ValueType value;
				size_t height;

				AVLNode *left;
				AVLNode *right;

				AVLNode(const KeyType &key, const ValueType &value);

				/** Must return `0`, `1`, or `2`. */
				size_t numChildren() const;

				/** Determines if the node is a leaf node. */
				bool isLeaf() const;

				/** Number of hops to deepest leaf node. */
				size_t getHeight() const;

				/** The difference between the heights of this node's children. */
				ssize_t getBalance() const;

				void rotateLeft(const AVLTree::Direction &childDir);
				void rotateRight(const AVLTree::Direction &childDir);
		};

	public:
		AVLTree();
		AVLTree(const AVLTree &other);
		~AVLTree();
		void operator=(const AVLTree &other);

		bool insert(const KeyType &key, ValueType value);
		bool remove(const KeyType &key);
		bool contains(const KeyType &key) const;

		std::optional<ValueType> get(const KeyType &key) const;
		ValueType & operator[](const KeyType &key);

		std::vector<KeyType> keys() const;
		std::vector<ValueType> findRange(const KeyType &low, const KeyType &high) const;

		size_t size() const;
		size_t getHeight() const;

		friend std::ostream & operator<<(std::ostream &os, const AVLTree &avlTree);

	private:
		AVLNode *root;
		size_t length;

		/* Recursive overloads for the methods declared above. */

		bool insert(AVLNode *current, const KeyType &key, ValueType &value);
		bool contains(const AVLNode *current, const KeyType &key) const;

		std::optional<ValueType> get(const AVLNode *current, const KeyType &key) const;
		ValueType & getValue(AVLNode *current, const KeyType &key);

		void grabKey(std::vector<KeyType> &keyList, const AVLNode *current) const;
		void grabValue(std::vector<ValueType> &valueList, const AVLNode *current, const KeyType &low, const KeyType &high) const;

		void insert(AVLNode *&current, const AVLNode *other);

		/* Helper methods for remove. */

		/** This overloaded remove will do the recursion to remove the node. */
		bool remove(AVLNode *&current, KeyType key);

		/** `removeNode` contains the logic for actually removing a node based on the number of children. */
		bool removeNode(AVLNode *&current);

		static void printDepth(std::ostream &os, const AVLNode *node, const size_t depth);
		friend std::ostream & operator<<(std::ostream &os, const AVLNode *node);

		void balanceNode(AVLNode *node, const Direction &childDir);

		friend constexpr ssize_t operator<=>(const ssize_t x, const Direction y) {
			return x - static_cast<ssize_t>(y);
		}

		void rotateLeft();
		void rotateRight();
};

#endif // AVLTREE_H
