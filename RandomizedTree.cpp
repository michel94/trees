#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <queue>
#include <time.h>

#define DEBUG 0

using namespace std;

string _TABS;

int randInt(int a, int b){
	return rand() % (b-a+1) + a;
}

template <class Key, class Value > 
class Node{
	public:
		Node();
		Node(Key);
		Node(Key, Value);

		Node<Key, Value>* left;
		Node<Key, Value>* right;
		Key key;
		Value value;
		int size;

		void updateHeight();
};

template <class Key, class Value>
Node<Key, Value>::Node(){
}

template <class Key, class Value>
Node<Key, Value>::Node(Key k){
	key = k;
}

template <class Key, class Value>
Node<Key, Value>::Node(Key k, Value v){
	key = k;
	value = v;
	left = NULL;
	right = NULL;
}

template <class Key, class Value>
class RandTree{
	public:
		RandTree();
		Node<Key, Value>* insert(Key, Value);
		Node<Key, Value>* find(Key);
		void remove(Key);
		
		void print() const;
		void printAux(Node<Key, Value>* n, int h) const;
		void printbfs();
		Value& operator [] (Key k);
		//template< RandTree<Key, Value> temp >
		//friend ostream& operator<< (ostream & out, const RandTree<Key, Value>& t);

	private:
		void removeByRef(Node<Key, Value>* n);
		Node<Key, Value>* tree;
		
		
};

template <class Key, class Value>
RandTree<Key, Value>::RandTree(){
	tree = new Node<Key, Value>();
}

template <class Key, class Value>
int split(Key k, Node<Key, Value>* t, Node<Key, Value>*& leftT, Node<Key, Value>*& rightT){
	if(t == NULL){
		leftT = NULL;
		rightT = NULL;
		return 0;
	}
	int s = 0;
	if(k < t->key){
		rightT = t;
		if(t->right != NULL)
			s += t->right->size;
		return split(k, t->left, leftT, rightT->left) + s;
	}else{
		leftT = t;
		if(t->left != NULL)
			s += t->left->size;
		return split(k, t->right, t->right, rightT) + s;
	}
}

template <class Key, class Value>
Node<Key, Value>* insertAtRoot(Node<Key, Value>*& t, Key k, Value v){
	Node<Key, Value> *leftT, *rightT;

	int s = split(k, t, leftT, rightT);
	t = new Node<Key, Value>(k, v);
	t->left = leftT;
	t->right = rightT;
	t->size = s + 1;
	
	return t;

}

template <class Key, class Value>
Node<Key, Value>* inserter(Node<Key, Value>*& t, Key k, Value v){
	if(t==NULL){
		t = new Node<Key, Value>(k, v);
		t->size = 1;
		return t;
	}
	int r = randInt(0, t->size);
	if(r == t->size){
		return insertAtRoot(t, k, v);
	}else if(k < t->key){
		return inserter(t->left, k, v);
	}else if(k > t->key){
		return inserter(t->right, k, v);
	}else{
		return t;
	}
	return NULL;
}

template <class Key, class Value>
Node<Key, Value>* RandTree<Key, Value>::insert(Key k, Value v){
	Node<Key, Value>* f = find(k);
	if(f != NULL)
		return f;
	return inserter(tree->left, k, v);
}

template <class Key, class Value>
Value& RandTree<Key, Value>::operator[](Key k){
	Value v = *(new Value());
	return insert(k, v)->value;
}

template <class Key, class Value>
Node<Key, Value>* RandTree<Key, Value>::find(Key k){
	Node<Key, Value>* n = tree->left;
	while(1){
		if(n == NULL)
			return NULL;
		if(k == n->key){
			return n;
		}else if(k < n->key){
			n = n->left;
		}else{
			n = n->right;
		}
	}
}

template <class Key, class Value>
void printNode(Node<Key, Value>* n){
	if(n != NULL)
		cout << n->key << " - " << n->value << endl;
	else
		cout << "NULL" << endl;
}

template <class Key, class Value>
void RandTree<Key, Value>::printAux(Node<Key, Value>* n, int h) const{
	if(n == NULL)
		return;

	if(DEBUG) _TABS += '\t';

	printAux(n->left, h+1);
	
	
	if(DEBUG) 
	if(_TABS.size())
		_TABS.resize (_TABS.size() - 1);

	if(n->value != 0)
		cout << _TABS << n->key << ": ";
	
	if(DEBUG) 
	_TABS += '\t';
	if(n->value != 0)
		cout << n->value << endl;

	printAux(n->right, h+1);

	if(DEBUG) 
	_TABS.resize (_TABS.size () - 1);
}

template <class Key, class Value>
void RandTree<Key, Value>::print() const{
	if(DEBUG) putchar('\n');
	printAux(tree->left, 0);
}

template <class Key, class Value>
ostream& operator<< (ostream & out, const RandTree<Key, Value>& t){
	t.print();
	return out;
}

int main(){

	srand(time(NULL));
	RandTree<string, int > tree;
	
	
	char rd[50];
	while(scanf("%s", rd) != EOF){
		string s (rd);
		if(s == "REMOVE"){
			scanf("%s", rd);
			string s2 (rd);
			transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
			tree[s2] = 0;
		}else{
			transform(s.begin(), s.end(), s.begin(), ::tolower);
			tree[s]++;
		}
	}
	tree.print();
	
	return 0;
}
