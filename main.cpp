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
	template <typename T>
	struct List {
		template <typename T>
		struct Node {
			T data;
			Node* next = nullptr;
			Node* prev = nullptr;
		};
		int size = 0;
		int currentIndex = 0;
		Node<T>* current = nullptr;
		Node<T>* head = nullptr;
		Node<T>* tail = nullptr;
		void toFirst() {
			if (!size) return;
			current = head;
			currentIndex = 0;
		}
		void toLast() {
			if (!size) return;
			current = tail;
			currentIndex = count() - 1;
		}
		void toNext() {
			if (!size) return;
			current = current->next;
			currentIndex++;
		}
		void toPrev() {
			if (!size) return;
			current = current->prev;
			currentIndex--;
		}
		void currentTo(int index) {
			if (!size) return;
			if ((currentIndex - index) > index) {
				toFirst();
			}
			if ((count() - 1 - index) < (index - currentIndex)) {
				toLast();
			}
			while (index > currentIndex) {
				toNext();
			}
			while (index < currentIndex) {
				toPrev();
			}
		}
		void removeCurrent() {
			if (!size) return;
			Node<T>* delNode = current;
			if ((delNode != head) and (delNode != tail)) {
				delNode->prev->next = delNode->next;
				delNode->next->prev = delNode->prev;
				current = current->next;
			}
			if (delNode == head) {
				head = delNode->next;
				if (delNode->next != nullptr) {
					delNode->next->prev = nullptr;
					current = current->next;
				}
			}
			if (delNode == tail) {
				tail = delNode->prev;
				if (delNode->prev != nullptr) {
					delNode->prev->next = nullptr;
					toPrev();
				}
			}
			delete delNode;
			size--;
			if (!size) current = nullptr;
		}
		void insertBeforeCurrent(T data) {
			if (!size) return;
			Node<T>* newNode = new Node<T>;
			size++;
			currentIndex++;
			newNode->data = data;
			newNode->next = current;
			newNode->prev = current->prev;
			if (current->prev != nullptr) current->prev->next = newNode;
			current->prev = newNode;
			if (current == head) { head = newNode; }
		}
		Node<T>* getCurrent(int index) {
			currentTo(index);
			return current;
		}

		void add(T data) {
			Node<T>* newNode = new Node<T>;
			newNode->data = data;
			if (!size) {
				currentIndex = 0;
				current = newNode;
				head = newNode;
				tail = newNode;
			}
			else {
				tail->next = newNode;
				newNode->prev = tail;
				tail = newNode;
			}
			size++;
		}
		void insert(int index, T data) {
			if ((!size) or (index == size)) {
				add(data);
			}
			else {
				currentTo(index);
				insertBeforeCurrent(data);
			}
		}
		void removeAt(int index) {
			currentTo(index);
			removeCurrent();
		}
		T elementAt(int index) {
			currentTo(index);
			return current->data;
		}
		int count() {
			return size;
		}
		void clear() {
			for (int i = size; i > 0; i--) {
				removeCurrent();
			}
		}
	};
	enum Order { Prefix, Infix, Postfix, LevelsUpLeft, LevelsUpRight, LevelsDownLeft, LevelsDownRight };
	int size = 0;
	Node* root = nullptr;
	void traverse(int* array, Order order) {
		if (root == nullptr) return;
		int index = 0;
		List<Node*> list;
		list.add(root);
		while (index != size) {
			for (int i = list.count(); i > 0; i--) {
				if (order == LevelsUpLeft || order == LevelsDownRight) {
					if (list.elementAt(0)->left != nullptr) {
						list.add(list.elementAt(0)->left);
					}
				}
				if (list.elementAt(0)->right != nullptr) {
					list.add(list.elementAt(0)->right);
				}
				if (order == LevelsUpRight || order == LevelsDownLeft) {
					if (list.elementAt(0)->left != nullptr) {
						list.add(list.elementAt(0)->left);
					}
				}
				array[index] = list.elementAt(0)->value;
				index++;
				list.removeAt(0);
			}
		}
		if (order == LevelsDownLeft || order == LevelsDownRight) {
			reverse(array, array + size);
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
		if (current == nullptr) return;
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
		if (order > 2) {
			traverse(array, order);
		}
		else {
			req(array, index, order, root);
		}
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
	int* arr = new int[tree.count()];
	arr = tree.ToArray(tree.Prefix);
	for (int i = 0; i < tree.count(); i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
	arr = tree.ToArray(tree.Infix);
	for (int i = 0; i < tree.count(); i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
	arr = tree.ToArray(tree.Postfix);
	for (int i = 0; i < tree.count(); i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
	arr = tree.ToArray(tree.LevelsUpLeft);
	for (int i = 0; i < tree.count(); i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
	arr = tree.ToArray(tree.LevelsUpRight);
	for (int i = 0; i < tree.count(); i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
	arr = tree.ToArray(tree.LevelsDownLeft);
	for (int i = 0; i < tree.count(); i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
	arr = tree.ToArray(tree.LevelsDownRight);
	for (int i = 0; i < tree.count(); i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
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