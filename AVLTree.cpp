#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <queue>

using namespace std;

string tabs;

template <class Key, class Value > 
class Node{
	public:
		Node();
		Node(Key);
		Node(Key, Value);

		Node<Key, Value>* left;
		Node<Key, Value>* right;
		Node<Key, Value>* parent;
		Key key;
		Value value;
		int height;

		int maxHeight();
		void rotateLeft();
		void rotateRight();
		void rotateDoubleLeft();
		void rotateDoubleRight();
		int balance();
		int rotate();
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
	parent = NULL;
	height = 0;
}

template <class Key, class Value>
class Tree{
	public:
		Tree();
		Node<Key, Value>* insert(Key, Value);
		Node<Key, Value>* find(Key);
		void remove(Key);
		Node<Key, Value>* left(Node<Key, Value>* n);
		void print() const;
		void printAux(Node<Key, Value>* n, int h) const;
		void printbfs();
		Value& operator [] (Key k);
		//template< Tree<Key, Value> temp >
		//friend ostream& operator<< (ostream & out, const Tree<Key, Value>& t);

	private:
		void removeByRef(Node<Key, Value>* n);
		Node<Key, Value>* tree;
		
		
};

template <class Key, class Value>
Tree<Key, Value>::Tree(){
	tree = new Node<Key, Value>();
	tree->parent = NULL;
}

template <class Key, class Value>
void Node<Key, Value>::rotateLeft(){
	/*A
	   \
	   B
	    \
	     C
	*/

	Node<Key, Value>* A, *B;
	A = this;
	B = right;
	
	pointParent(A, B);
	B -> parent = A -> parent;

	A -> right = B -> left;
	if(B -> left != NULL)
		B -> left -> parent = A;
		
	B -> left = A;
	A -> parent = B;
	
	A -> height = A -> maxHeight();
	B -> height = B -> maxHeight();
	
	
}

template <class Key, class Value>
void Node<Key, Value>::rotateDoubleLeft(){
	/*
	 A
	  \
	   B
	  /
     C
	*/
	right->rotateRight();
	rotateLeft();
}

template <class Key, class Value>
void Node<Key, Value>::rotateRight(){
	/*    A
	     /
		B
	   /
	  C
	 */

	Node<Key, Value> *A, *B;
	A = this;
	B = left;
	

	pointParent(A, B);
	B -> parent = A -> parent;

	A -> left = B -> right;
	if(B -> right != NULL)
		B -> right -> parent = A;
		
	B -> right = A;
	A -> parent = B;

	A -> height = A->maxHeight();
	B -> height = B->maxHeight();

}

template <class Key, class Value>
void Node<Key, Value>::rotateDoubleRight(){
	/*
	   A
      /
     B
      \
      C
	*/

	left->rotateLeft();
	rotateRight();

}

template <class Key, class Value>
int Node<Key, Value>::balance(){
	if(left == NULL && right == NULL)
		return 0;
	else if(left == NULL)
		return right -> height + 1;
	else if(right == NULL)
		return -(left -> height + 1);
	else{
		return -left -> height + right -> height;
	}

}

template <class Key, class Value>
int Node<Key, Value>::rotate(){

	if(balance() == 2){
		if(right->balance() > 0){
			rotateLeft();
			return 1;
		}else{
			rotateDoubleLeft();
			return 1;
		}
	}else if(balance() == -2){
		if(left->balance() < 0){
			rotateRight();
			return 1;
		}else{
			rotateDoubleRight();
			return 1;
		}
	}else{
		return 0;
	}

}

template <class Key, class Value>
int Node<Key, Value>::maxHeight(){
	
	if(left == NULL && right == NULL)
		return 0;
	else if(left == NULL)
		return right -> height + 1;
	else if(right == NULL)
		return left -> height + 1;
	else
		return left -> height > right -> height ? left -> height + 1 : right -> height + 1;
	
}

template <class Key, class Value>
void Node<Key, Value>::updateHeight(){
	
	int height;
	Node<Key, Value>* tree = this;
	while(tree->parent != NULL){
		height = tree->maxHeight();
		
		if(height == tree -> height){
			break;
		}else
			tree -> height = height;
		tree->rotate();

		tree = tree -> parent;
	}
}

template <class Key, class Value>
Node<Key, Value>* Tree<Key, Value>::insert(Key k, Value v){
	if(tree->left == NULL){
		tree->left = new Node<Key, Value>(k, v);
		tree->left->height = 0;
		tree->left->parent = tree;
		return tree->left;
	}else{
		Node<Key, Value>* n = tree->left;
		while(1){
			if(k == n->key){
				return n;
			}else if(k < n->key){
				if(n->left == NULL){
					n->left = new Node<Key, Value>(k, v);
					Node<Key, Value>* rt = n->left;
					n->left->parent = n;
					n->updateHeight();
					return rt;
				}else{
					n = n->left;
				}
			}else{
				if(n->right == NULL){
					n->right = new Node<Key, Value>(k, v);
					Node<Key, Value>* rt = n->right;
					n->right->parent = n;
					n->updateHeight();
					return rt;
				}else{
					n = n->right;
				}
			}
		}
	}
	
}

template <class Key, class Value>
Value& Tree<Key, Value>::operator[](Key k){
	Value v = *(new Value());
	return insert(k, v)->value;
}


template <class Key, class Value>
Node<Key, Value>* Tree<Key, Value>::find(Key k){
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
Node<Key, Value>* Tree<Key, Value>::left(Node<Key, Value>* n){
	if(n == NULL) return NULL;
	while(n->left != NULL)
		n = n->left;
	return n;
}

template <class Key, class Value>
void printNode(Node<Key, Value>* n){
	if(n != NULL)
		cout << n->key << " - " << n->value << endl;
	else
		cout << "NULL" << endl;
}

template <class Key, class Value>
void pointParent(Node<Key, Value>* c, void* pointer){
	Node<Key, Value>* d = (Node<Key, Value>*)pointer;
	Node<Key, Value>* p = c->parent;
	if(p->left!=NULL){
		if(p->left->key == c->key){
			p->left = d;
		}else{
			p->right = d;
		}
	}else{
		p->right = d;
	}
}

template <class Key, class Value>
void Tree<Key, Value>::remove(Key k){
	Node<Key, Value>* n = find(k);
	if(n == NULL)
		return;
	if(n->right==NULL && n->left==NULL){
		//printf("CASE 0\n");
		pointParent(n, NULL);
		n->parent->updateHeight();
		delete n;
	}else if(n->right == NULL){
		//printf("CASE 1\n");
		pointParent(n, n->left);
		n->left->parent = n->parent;
		n->parent->updateHeight();
		delete n;
	}else if(n->left == NULL){
		//printf("CASE 2\n");
		pointParent(n, n->right);
		n->right->parent = n->parent;
		n->parent->updateHeight();
		delete n;
	}else{
		Node<Key, Value>* rm = n -> left;
		while(rm -> right != NULL)
			rm = rm -> right;
		
		if(rm != n->left){
			//printf("CASE 3 - 1\n");
			n -> key = rm -> key;
			n -> value = rm -> value;

			rm -> parent -> right = rm -> left;
			if(rm -> left != NULL)
				rm -> left -> parent = rm -> parent;

			rm->parent->updateHeight();
			delete rm;
		}else{
			//printf("CASE 3 - 2\n");
			n -> key = rm -> key;
			n -> value = rm -> value;
			n->left = n->left->left;
			if(n->left != NULL){
				n->left->parent = n;
				n->left->updateHeight();
			}
			delete rm;
		}
	}
	
}

template <class Key, class Value>
void Tree<Key, Value>::removeByRef(Node<Key, Value>* n){
	
	if(n == NULL)
		return;
	if(n->right==NULL && n->left==NULL){
		//printf("CASE 0\n");
		pointParent(n, NULL);
		n->parent->updateHeight();
		delete n;
	}else if(n->right == NULL){
		//printf("CASE 1\n");
		pointParent(n, n->left);
		n->left->parent = n->parent;
		n->parent->updateHeight();
		delete n;
	}else if(n->left == NULL){
		//printf("CASE 2\n");
		pointParent(n, n->right);
		n->right->parent = n->parent;
		n->parent->updateHeight();
		delete n;
	}else{
		Node<Key, Value>* rm = n -> left;
		while(rm -> right != NULL)
			rm = rm -> right;
		
		if(rm != n->left){
			//printf("CASE 3 - 1\n");
			rm -> parent -> right = rm -> left;
			if(rm -> left != NULL)
				rm -> left -> parent = rm -> parent;

			n -> key = rm -> key;
			n -> value = rm -> value;
			
			pointParent(rm, NULL);
			rm->parent->updateHeight();
			delete rm;
		}else{
			//printf("CASE 3 - 2\n");
			n -> key = rm -> key;
			n -> value = rm -> value;
			n->left = n->left->left;
			if(n->left != NULL){
				n->left->parent = n;
				n->left->updateHeight();
			}
			delete rm;
		}
	}
	
}

template <class Key, class Value>
void Tree<Key, Value>::printAux(Node<Key, Value>* n, int h) const{
	if(n == NULL)
		return;

	tabs += '\t';

	printAux(n->left, h+1);
	//if(n->value != 0)
	
	if(tabs.size())
		tabs.resize (tabs.size() - 1);

	cout << tabs << n->key << ": ";
	tabs += '\t';
	cout << n->value << endl;
	

	printAux(n->right, h+1);


	tabs.resize (tabs.size () - 1);
}

template <class Key, class Value>
void Tree<Key, Value>::print() const{
	putchar('\n');
	printAux(tree->left, 0);
}

template <class Key, class Value>
ostream& operator<< (ostream & out, const Tree<Key, Value>& t){
	t.print();
	return out;
}

/*
template <class Key, class Value>
void Tree<Key, Value>::printbfs(){
	queue< Node<string, int> > q;
	q.push(*(tree->left));
	while(!q.empty()){
		Node<Key, Value> n = q.front();
		printNode(&n);
		printf("(");
			printNode(n.left);
			printNode(n.right);
		printf(") - ");
		printf("%d\n", n.height);
		if(n.left != NULL){
			q.push(*(n.left));
		}
		if(n.right != NULL){
			q.push(*(n.right));
		}
		q.pop();
		
	}
}*/

int main(){

	Tree<string, Tree<string, int> > tree;
	tree["ze"]["couve"] = 4;
	tree["ze"]["alface"] = 9;
	tree["ze"]["pepino"] = 2;
	tree["ze"]["tomate"] = 6;
	tree["ze"]["batata"] = 1;
	tree["ze"]["cenoura"] = 5;

	tree["michel"]["couve"] = 47;
	tree["michel"]["alface"] = 92;
	tree["michel"]["pepino"] = 26;
	tree["michel"]["tomate"] = 63;
	tree["michel"]["batata"] = 13;
	tree["michel"]["cenoura"] = 51;
	//cout << tree["nabo"]["couve"] << endl;
	//tree.print();
	cout << tree << endl;
	/*char rd[50];
	while(scanf("%s", rd) != EOF){
		string s (rd);
		if(s == "REMOVE"){
			scanf("%s", rd);
			string s2 (rd);
			transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
			tree.remove(s2);
		}else{
			transform(s.begin(), s.end(), s.begin(), ::tolower);
			tree[s]++;
		}
	}
	tree.print();*/
	
	return 0;
}
