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
		bool contains(AVLNode *current, const KeyType &key) const;

		/* Helper methods for remove. */

		/** This overloaded remove will do the recursion to remove the node. */
		bool remove(AVLNode *&current, KeyType key);

		/** `removeNode` contains the logic for actually removing a node based on the number of children. */
		bool removeNode(AVLNode *&current);

		/** You will implement this, but it is needed for `removeNode()`. */
		void balanceNode(AVLNode *&node);

		static void printDepth(std::ostream &os, const AVLNode *node, const size_t depth);
		friend std::ostream & operator<<(std::ostream &os, const AVLNode *node);
};

#endif // AVLTREE_H
