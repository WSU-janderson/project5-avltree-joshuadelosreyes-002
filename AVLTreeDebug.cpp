/**
 *	Driver code for testing your AVL Tree.
 *	This is NOT the test code for grading,
 *	instead for you to get an idea of how to test the tree.
 */

#include <iostream>
#include <ranges>
#include <vector>
#include "AVLTree.h"

using namespace std;

#define RUN_TEST 1
#define COPY_TEST 0
#define MEMLEAK_TEST 0

int main() {
#if defined(RUN_TEST) && (RUN_TEST != 0)
	AVLTree tree;
	bool insertResult;
	insertResult = tree.insert("F", 'F');
	insertResult = tree.insert("F", 'F'); // Expected false, no duplicates allowed.
	insertResult = tree.insert("K", 'K');
	insertResult = tree.insert("X", 'X'); // single rotate left
	cout << endl << endl;
	cout << tree << endl;

	insertResult = tree.insert("C", 'C');
	insertResult = tree.insert("A", 'A'); // single rotate right
	cout << endl << endl;
	cout << tree << endl;

	insertResult = tree.insert("D", 'D'); // double rotate right
	cout << endl << endl;
	cout << tree << endl;

	insertResult = tree.insert("R", 'R'); // double rotate left
	cout << endl << endl;
	cout << tree << endl;

	insertResult = tree.insert("V", 'V');
	insertResult = tree.insert("A", 'A'); // false, duplicate
	insertResult = tree.insert("Z", 'Z');
	insertResult = tree.insert("M", 'M');
	insertResult = tree.insert("D", 'D'); // false, duplicate
	cout << endl << endl;
	cout << tree << endl;

	// size and getHeight
	cout << "tree size: " << tree.size() << endl; 			// Expected 10
	cout << "tree height: " << tree.getHeight() << endl;	// Expected 3
	cout << endl;

	// contains
	bool containsResult;
	containsResult = tree.contains("A"); // Expected true
	containsResult = tree.contains("N"); // Expected false

	// get
	optional<size_t> getResult;

	getResult = tree.get("A"); // Expected 65
	cout << "A: " << getResult.value() << endl;

	getResult = tree.get("C"); // Expected 67
	cout << "C: " << getResult.value() << endl;

	getResult = tree.get("Q"); // getResult has no value.
	cout << "Q: " << getResult.has_value() << endl; // print 0
	cout << endl;

	// findRange
	vector<size_t> rangeTest = tree.findRange("D", "W");
	// 70 68 82 75 77 86
	for (auto val : rangeTest) {
		cout << val << " ";
	}
	cout << endl << endl;

	// operator[]
	tree["A"] = 108;
	cout << tree << endl;
	cout << endl;

	// remove
	bool removeResult;
	removeResult = tree.remove("A"); // "A" is a leaf.
	cout << endl << endl;
	cout << tree << endl;

	removeResult = tree.remove("C"); // "C" has one child, single rotate left.
	cout << endl << endl;
	cout << tree << endl;

	removeResult = tree.remove("F"); // "F" has two children.
	cout << endl << endl;
	cout << tree << endl;

	removeResult = tree.remove("V");
	removeResult = tree.remove("X");
	removeResult = tree.remove("Z"); // double rotate right
	cout << endl << endl;
	cout << tree << endl;

#if defined(COPY_TEST) && (COPY_TEST != 0)
	AVLTree tree2(tree);

	cout << "tree:\n" << tree << "\n";
	cout << "tree2:\n" << tree2 << "\n";

	tree2.insert("AA", 720);
	tree2.insert("AB", 730);
	tree2.insert("BA", 210);
	tree2.insert("BB", 350);

	cout << "tree:\n" << tree << "\n";
	cout << "tree2:\n" << tree2 << "\n";

	tree = tree2;

	tree.remove("BB");
	tree.remove("BA");

	cout << "tree:\n" << tree << "\n";
	cout << "tree2:\n" << tree2 << "\n";
#endif // COPY_TEST

#if defined(MEMLEAK_TEST) && (MEMLEAK_TEST != 0)
	{
		AVLTree tree3;
		char key[4];
		for (key[0] = 'A'; key[0] <= 'Z'; ++key[0]) {
			for (key[1] = 'A'; key[1] <= 'Z'; ++key[1]) {
				for (key[2] = 'A'; key[2] <= 'Z'; ++key[2]) {
					for (key[3] = 'A'; key[3] <= 'Z'; ++key[3]) {
						tree3.insert(std::string{key}, key[0] + key[1] + key[2] + key[3]);
					}
				}
			}
		}

		cout << tree3.size() << "\n";
	}
#endif // MEMLEAK_TEST
#endif // RUN_TEST

	return 0;
}
