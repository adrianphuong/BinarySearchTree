#include "Dictionary.h"

using namespace std;

Dictionary::Dictionary() {
	nil = new Node("", -1);
	root = nil;
	current = nil;
	num_pairs = 0;
}

Dictionary::Node::Node(keyType k, valType v) {
	key = k;
	val = v;
	parent = nullptr;
	left = nullptr;
	right = nullptr;
}

Dictionary::Dictionary(const Dictionary &D) {
	nil = new Node("", -1);
	root = nil;
	current = nil;
	num_pairs = 0;
	preOrderCopy(D.root, D.nil);
}

Dictionary::~Dictionary() {
	postOrderDelete(root);
	delete nil;
}

void Dictionary::inOrderString(std::string& s, Node* R) const{
	if (R != nil) {
		inOrderString(s, R->left);
		s.append(R->key);
		s.append(" : ");
		s.append(std::to_string(R->val));
		s.append("\n");
		inOrderString(s, R->right);
	}
}

void Dictionary::preOrderString(std::string &s, Node *R) const {
	if(R != nil) {
		s.append(R->key);
		s.append("\n");
		preOrderString(s, R->left);
		preOrderString(s, R->right);
	}
}

int Dictionary::size() const {
	return num_pairs;
}

Dictionary::Node* Dictionary::search(Node *R, keyType k) const {
	if (R == nil || R->key == k) {
		return R;
	}
	if(k < R->key) {
		return search(R->left, k);
	}
	else {
		return search(R->right, k);
	}
}

bool Dictionary::contains(keyType k) const {
	if(search(root, k) != nil) {
		return true;
	}
	else {
		return false;
	}
}

valType& Dictionary::getValue(keyType k) const {
	if(contains(k)) {
		Node* temp = search(root, k);
		return temp->val;
	}
	else {
		throw std::logic_error("Current Undefined");
		return nil->val;
	}
}

bool Dictionary::hasCurrent() const {
	if(current == nil) {
		return false;
	}
	else {
		return true;
	}
}

keyType Dictionary::currentKey() const {
	if(hasCurrent()) {
		return current->key;
	}
	else {
		throw std::logic_error("Current undefined");
		return nil->key;
	}
}

valType& Dictionary::currentVal() const {
	if(hasCurrent() == true) {
		return current->val;
	}
	else {
		throw std::logic_error("Current undefined");
		return nil->val;
	}
}

void Dictionary::postOrderDelete(Node *R) {
	if(R != nil) {
		if(R == root) {
			current = nil;
			root = nil;
		}
	}
	else {
		return;
	}
	postOrderDelete(R->left);
	postOrderDelete(R->right);
	delete R;
}

void Dictionary::clear() {
	postOrderDelete(root);
	num_pairs = 0;
	current = nil;
	root = nil;
}

void Dictionary::setValue(keyType k, valType v) {
	if(root != nil && root->key == k) {
		root->val = v;
		return;
	}
	Node* N = new Node(k, v);       
        N->left = nil;
        N->right = nil;
        N->parent = nil;
	Node *Rtemp = root;
	Node *Ntemp = nil;
	while(Rtemp != nil) {
		Ntemp = Rtemp;
		if(k == Rtemp->key) {
			Rtemp->val = v;
			return;
		}
		else if(k < Rtemp->key) {
			Rtemp = Rtemp->left;
		}
		else {
			Rtemp = Rtemp->right;
		}
	}
	N->parent = Ntemp;
	if(Ntemp == nil) {
		root = N;
	}
	else if(N->key < Ntemp->key) {
		Ntemp->left = N;
	}
	else {
		Ntemp->right = N;
	}
	num_pairs++;
}

void Dictionary::transplant(Node *u, Node *v) { // transplant from pseudcode provided
	if(u->parent == nil) {
		root = v;
	}
	else if (u == u->parent->left) {
		u->parent->left = v;
	}
	else {
		u->parent->right = v;
	}
	if(v != nil) {
		v->parent = u->parent;
	}
}

void Dictionary::remove(keyType k) {
	if(search(root, k) == current) {
		current = nil;
	}
		Node *N = search(root, k);
		if(N->left == nil) {
			transplant(N, N->right);
			delete N;
		}
		else if (N->right == nil) {
			transplant(N, N->left);
			delete N;
		}
		else {
			Node *min = findMin(N->right);
			if(min->parent != N) {
				transplant(min, min->right);
				min->right = N->right;
				min->right->parent = N;
			}
			transplant(N, min);
			min->left = N->left;
			min->left->parent = N;
			delete N;
		}
	num_pairs -= 1;
}

void Dictionary::preOrderCopy(Node *R, Node *N) {
	if(R != N && R != nil) {
		setValue(R->key, R->val);
		preOrderCopy(R->left, N);
		preOrderCopy(R->right, N);
	}
}

Dictionary::Node* Dictionary::findMin(Node *R) {	
	if(size() > 0) {
		while(R->left != nil) {
			R = R->left;
		}
		return R;
	}
	else {
		return nil;
	}
}

void Dictionary::begin() {
	if(size() > 0) {
		current = findMin(root);
	}
}

Dictionary::Node* Dictionary::findMax(Node *R) {
	if(size() > 0) {
		while(R->right != nil) {
			R = R->right;
		}
		return R;
	}
	else {
		return nil;
	}
}

void Dictionary::end() {
	if(size() > 0) {
		current = findMax(root);
	}
}

Dictionary::Node* Dictionary::findNext(Node *N) {
	if(N->right != nil) {
		return findMin(N->right);
	}
	while(N->parent != nil && N == N->parent->right) {
		N = N->parent;
		N->parent = N->parent->parent;
	}
	return N->parent;
}

void Dictionary::next() {
	if(hasCurrent()) {
		if(findMax(root) != current) {
			current = findNext(current);
		}
		else {
			current = nil;
		}
	}
}

Dictionary::Node* Dictionary::findPrev(Node *N) {
	if(N->left != nil) {
		return findMax(N->left);
	}
	while(N->parent != nil && N == N->parent->left) {
		N = N->parent;
		N->parent = N->parent->parent;
	}
	return N->parent;
}

void Dictionary::prev() {
	if(hasCurrent()) {
		if(findMin(root) != current) {
			current = findPrev(current);
		}
		else {
			current = nil;
		}
	}
}

std::string Dictionary::to_string() const{
	std::string s = "";
	inOrderString(s, root);
	return s;
}

std::string Dictionary::pre_string() const {
	std::string s = "";
	preOrderString(s, root);
	return s;
}

bool Dictionary::equals(const Dictionary &D) const {
	int Asize = size();
	int Bsize = D.size();
	std::string A = to_string();
	if((Asize != Bsize) || (A != D.to_string())) {
		return false;
	}
	else {
		return true;
	}
}

std::ostream& operator<<(std::ostream &stream, Dictionary &D) {
	return stream << D.to_string();
}

bool operator==(const Dictionary &A, const Dictionary &B) {
	return A.Dictionary::equals(B);
}

Dictionary& Dictionary::operator=(const Dictionary &D) {
	if(this != &D) {
		if(num_pairs > 0) {
			postOrderDelete(root);
		}
		preOrderCopy(D.root, D.nil);
	}
	return *this;
}
