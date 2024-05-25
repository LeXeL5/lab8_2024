#include <iostream>
using namespace std;

struct Node {
	Node* left = nullptr;
	Node* right = nullptr;
	Node* up = nullptr;
	int value = 0;
	unsigned short depth = 1;
	unsigned short minDepth = 0;
};
struct Tree {
	struct Queue {
		struct node {
			Node* data = nullptr;
			node* next_node = nullptr;
		};
		node* head = nullptr;
		node* tail = nullptr;
		int counter = 0;
		void queue(Node* Node) {
			node* new_node = new node();
			new_node->data = Node;
			if (counter == 0) {
				head = new_node;
				tail = new_node;
			}
			else {
				tail->next_node = new_node;
				tail = new_node;
			}
			counter++;
		}
		int unqueue() {
			if (counter > 0) {
				node* new_head = head->next_node;
				int value = head->data->value;
				delete head;
				head = new_head;
				counter--;
				return value;
			}
			else { return 0; }
		}
		int count() {
			return counter;
		}
	};
	enum Order { Prefix, Infix, Postfix, LevelsUpLeft, LevelsUpRight, LevelsDownLeft, LevelsDownRight };
	int size = 0;
	Node* root = nullptr;
	void traverseLevelsUpLeft(int* array, int& index, Node* current) {
		if (current == nullptr) return;
		Queue* queue = new Queue;
		queue->queue(current);
		while (index != size) {
			for (int i = queue->count(); i > 0; i--) {
				if (queue->head->data->left != nullptr) {
					queue->queue(queue->head->data->left);
				}
				if (queue->head->data->right != nullptr) {
					queue->queue(queue->head->data->right);
				}
				array[index] = queue->unqueue();
				index++;
			}
		}
	}
	void traverseLevelsUpRight(int* array, int& index, Node* current) {
		if (current == nullptr) return;
		Queue* queue = new Queue;
		queue->queue(current);
		while (index != size) {
			for (int i = queue->count(); i > 0; i--) {
				if (queue->head->data->right != nullptr) {
					queue->queue(queue->head->data->right);
				}
				if (queue->head->data->left != nullptr) {
					queue->queue(queue->head->data->left);
				}
				array[index] = queue->unqueue();
				index++;
			}
		}
	}
	void renegadeBalancer(Node* current) {
		if (current == nullptr) return;
		while (abs(getDepth(current, true) - getDepth(current, false)) > 1) {
			doubleTurn(current);
		}
	}
	void additionalTurn(Node* current) {
		if (current == nullptr) return;
		if (getDepth(current, true) > getDepth(current, false)) {
			if (getMinDepth(current, true) == getDepth(current, false)) {
				turn(current->left, true);
				if (turn(current, false)) current = current->up;
				doubleTurn(current->left);
				doubleTurn(current->right);
			}
		}
		else if (getDepth(current, true) < getDepth(current, false)) {
			if (getDepth(current, true) == getMinDepth(current, false)) {
				turn(current->right, false);
				if (turn(current, true)) current = current->up;
				doubleTurn(current->right);
				doubleTurn(current->left);
			}
		}
	}
	int getMinDepth(Node* current, bool isLeft) {
		if (isLeft) {
			if (current->left != nullptr) return current->left->minDepth;
		}
		else {
			if (current->right != nullptr) return current->right->minDepth;
		}
		return 0;
	}
	int getDepth(Node* current, bool isLeft) {
		if (isLeft) {
			if (current->left != nullptr) return current->left->depth;
		}
		else {
			if (current->right != nullptr) return current->right->depth;
		}
		return 0;
	}
	bool doubleTurn(Node* current) {
		if (getDepth(current, true) - 1 > getDepth(current, false)) {
			if (getDepth(current->left, true) < getDepth(current->left, false)) {
				turn(current->left, true);
			}
			turn(current, false);
			return true;
		}
		else if (getDepth(current, false) - 1 > getDepth(current, true)) {
			if (getDepth(current->right, false) < getDepth(current->right, true)) {
				turn(current->right, false);
			}
			turn(current, true);
			return true;
		}
		return false;
	}
	void Balance(Node* current = nullptr) {
		if (current == nullptr) current = root;
		if (current == nullptr) return;
		if (current->depth < 3) return;
		if (current->left != nullptr) Balance(current->left);
		if (current->right != nullptr) Balance(current->right);
		if (current->depth < 3) return;
		if (doubleTurn(current)) {
			renegadeBalancer(current);
		}
		else {
			additionalTurn(current);
		}
	}
	void depthFixer(Node* current) {
		while (current != nullptr) {
			unsigned short left = 0;
			unsigned short right = 0;
			unsigned short minLeft = 0;
			unsigned short minRight = 0;
			if (current->left != nullptr) {
				left = current->left->depth;
				minLeft = current->left->minDepth;
			}
			if (current->right != nullptr) {
				right = current->right->depth;
				minRight = current->right->minDepth;
			}
			current->depth = max(left, right) + 1;
			current->minDepth = (min(minLeft, minRight) + 1);
			current = current->up;
		}
	}
	void req(int* array, int& index, Order order, Node* current) {
		if (order == Prefix) {
			array[index] = current->value;
			index++;
		}
		req(array, index, order, current->left);
		if (order == Infix) {
			array[index] = current->value;
			index++;
		}
		req(array, index, order, current->right);
		if (order == Postfix) {
			array[index] = current->value;
			index++;
		}
	}
	int* ToArray(Order order = Infix) {
		int* array = new int[count()];
		int index = 0;
		if (order == LevelsUpLeft) {
			traverseLevelsUpLeft(array, index, root);
			return array;
		}
		if (order == LevelsUpRight) {
			traverseLevelsUpRight(array, index, root);
			return array;
		}
		if (order == LevelsDownLeft) {
			traverseLevelsUpRight(array, index, root);
			reverse(array, array + size);
			return array;
		}
		if (order == LevelsDownRight) {
			traverseLevelsUpLeft(array, index, root);
			reverse(array, array + size);
			return array;
		}
		req(array, index, order, root);
		return array;
	}
	bool turn(Node* current, bool isLeft) {
		if (current == nullptr) return false;
		Node* parent = current->up;
		Node* child = nullptr;
		Node* grandchild = nullptr;
		if (isLeft) {
			child = current->right;
			if (child == nullptr) return false;
			grandchild = child->left;
			child->left = current;
			current->right = grandchild;
		}
		else {
			child = current->left;
			if (child == nullptr) return false;
			grandchild = child->right;
			child->right = current;
			current->left = grandchild;
		}
		if (parent == nullptr) {
			root = child;
		}
		else {
			if (parent->left == current) {
				parent->left = child;
			}
			else {
				parent->right = child;
			}
		}
		if (grandchild != nullptr) grandchild->up = current;
		current->up = child;
		child->up = parent;
		depthFixer(current);
		return true;
	}
	void ToLeft(int value) {
		Node* current = goTo(value);
		turn(current, true);
	}
	void ToRight(int value) {
		Node* current = goTo(value);
		turn(current, false);
	}
	Node* goTo(int value) {
		Node* current = root;
		while (current != nullptr) {
			if (value == current->value) {
				return current;
			}
			if (value < current->value) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}
		return nullptr;
	}
	Node* goToParent(int value) {
		Node* parent = nullptr;
		Node* current = root;
		while (current != nullptr) {
			if (value == current->value) {
				return nullptr;
			}
			parent = current;
			if (value < current->value) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}
		return parent;
	}
	void add(int value) {
		if (!size) {
			Node* newNode = new Node;
			newNode->value = value;
			size++;
			root = newNode;
			return;
		}
		Node* parent = goToParent(value);
		if (parent == nullptr) return;
		Node* newNode = new Node;
		size++;
		newNode->up = parent;
		newNode->value = value;
		if (value > parent->value) {
			parent->right = newNode;
		}
		else {
			parent->left = newNode;
		}
		depthFixer(newNode);
	}
	void remove(int value) {
		Node* current = goTo(value);
		if (current == nullptr) return;
		Node* parent = current->up;
		if ((current->right != nullptr) && (current->left != nullptr)) {
			parent = current;
			Node* min = current->right;
			while (min->left != nullptr) {
				parent = min;
				min = min->left;
			}
			current->value = min->value;
			current = min;
		}
		Node* child = nullptr;
		if (current->left != nullptr) {
			child = current->left;
		}
		else {
			child = current->right;
		}
		if (current != root) {
			if (parent->left == current) {
				parent->left = child;
			}
			else if (parent->right == current) {
				parent->right = child;
			}
		}
		else {
			root = child;
		}
		if (child != nullptr) child->up = parent;
		depthFixer(current);
		delete current;
		size--;
	}
	void clear(Node* current) {
		if (current == nullptr) return;
		clear(current->left);
		clear(current->right);
		delete current;
	}
	void clear() {
		clear(root);
		root = nullptr;
		size = 0;
	}
	bool contains(int value) {
		if (goTo(value) != nullptr) {
			return true;
		}
		else {
			return false;
		}
	}
	int count() {
		return size;
	}
};
int main() {
	Tree tree;
	int input = 0;
	cout << "Enter numbers to add (sequence end sign 0):" << endl;
	cin >> input;
	while (input) {
		tree.add(input);
		cin >> input;
	}
	cout << "Enter numbers to search (sequence end sign 0):" << endl;
	cin >> input;
	while (input) {
		if (tree.contains(input)) {
			cout << "This number is on the tree" << endl;
		}
		else {
			cout << "This number is NOT on the tree" << endl;
		}
		cin >> input;
	}
	tree.clear();
	return 0;
}