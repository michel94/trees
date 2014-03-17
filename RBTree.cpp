#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <queue>

using namespace std;

#define BLACK 0
#define RED 1

#define DEBUG 0

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
		int color;

		int maxHeight();
		void rotateLeft();
		void rotateRight();
		void rotateDoubleLeft();
		void rotateDoubleRight();
		void balance();
		void validate();
		void updateHeight();
		bool isLeftChild();
		Node<Key, Value>* uncle();
		Node<Key, Value>* sibling();

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
}

template <class Key, class Value>
Node<Key, Value>* Node<Key, Value>::uncle(){
	if(parent == NULL){
		return NULL;
	}
	if(parent->parent == NULL){
		return NULL;
	}
	if(parent -> parent -> left == parent){	
		return parent->parent->right;
	}
	return parent->parent->left;

}

template <class Key, class Value>
Node<Key, Value>* Node<Key, Value>::sibling(){
	if(parent == NULL)
		return NULL;
	if(this == parent->left)
		return parent->right;
	return parent->left;



}

template <class Key, class Value>
class RBTree{
	public:
		RBTree();
		Node<Key, Value>* insert(Key, Value);
		Node<Key, Value>* find(Key);
		void remove(Key);
		Node<Key, Value>* left(Node<Key, Value>* n);
		void print();
		void printAux(Node<Key, Value>* n, int h);
		void printbfs();
		Value& operator [] (Key k);
		Node<Key, Value>* tree;
	private:
		void removeByRef(Node<Key, Value>* n);
		
		
};

template <class Key, class Value>
void blackrule(Node<Key, Value> *n, int nb){
	if(n == NULL)
		return;

	if(n->color == BLACK)
		nb++;

	if(n->left == NULL && n->right == NULL){
		printf("%d - ", nb);
		printNode(n);	
		return;
	}
	if(n->color == RED){
		if(n->left != NULL)
			if(n->left->color == RED)
				printf("ERRO\n");
		if(n->right != NULL)
			if(n->right->color == RED){
				printf("ERRO\n");
				printNode(n);
				printNode(n->right);
			}
	}

	blackrule(n->left, nb);
	blackrule(n->right, nb);

}

template <class Key, class Value>
RBTree<Key, Value>::RBTree(){
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
    Node<Key, Value>* p = parent;
	rotateRight();
	p->rotateLeft();
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
    Node<Key, Value>* p = parent;
	rotateLeft();
	p->rotateRight();

}

template <class Key, class Value>
void Node<Key, Value>::validate(){
	if(parent == NULL)
		return;

	if(parent->color == BLACK){
		if(DEBUG) printf("CASO 0\n");
		return;
	}	
	
	if(uncle() != NULL && uncle()->color == RED){
		if(DEBUG) printf("CASO 1\n");
		parent->parent->color = RED;
		parent->color = BLACK;
		uncle()->color = BLACK;
		parent->parent->validate();
	}else if(uncle() == NULL || uncle()->color == BLACK){
		if(isLeftChild() && !parent->isLeftChild()){
			if(DEBUG) printf("CASO2.1\n");
			color = BLACK;
			parent->color = RED;
			parent->parent->color = RED;
			if(uncle() != NULL){
				uncle()->color = BLACK;
			}
			parent->rotateDoubleLeft();
		}else if(!isLeftChild() && parent->isLeftChild()){
			if(DEBUG) printf("CASO2.2\n");
			color = BLACK;
			parent->color = RED;
			parent->parent->color = RED;
			if(uncle() != NULL){
				uncle()->color = BLACK;
			}
			parent->rotateDoubleRight();
		}else if(isLeftChild() && parent->isLeftChild()){
			if(DEBUG) printf("CASO3.1\n");
			color = RED;
			parent->color = BLACK;
			parent->parent->color = RED;
			if(uncle() != NULL){
				uncle()->color = BLACK;
			}
			parent->parent->rotateRight();
		}else if(!isLeftChild() && !parent->isLeftChild()){
			if(DEBUG) printf("CASO3.2\n");
			color = RED;
			parent->color = BLACK;
			parent->parent->color = RED;
			if(uncle() != NULL){
				uncle()->color = BLACK;
			}
			parent->parent->rotateLeft();
		}
	}

}

template <class Key, class Value>
bool Node<Key, Value>::isLeftChild(){
	return parent->left == this;
}

template <class Key, class Value>
Node<Key, Value>* RBTree<Key, Value>::insert(Key k, Value v){
	if(tree->left == NULL){
		tree->left = new Node<Key, Value>(k, v);
		tree->left->color = BLACK;
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
					rt->parent = n;
					rt->color = RED;
					rt->validate();
					tree->left->color = BLACK;
					//printf("COLOR %d\n", n->left->color);

					return rt;
				}else{
					n = n->left;
				}
			}else{
				if(n->right == NULL){
					n->right = new Node<Key, Value>(k, v);
					Node<Key, Value>* rt = n->right;
					rt->parent = n;
					rt->color = RED;
					rt->validate();
					tree->left->color = BLACK;
					//printf("COLOR %d\n", n->right->color);
					return rt;
				}else{
					n = n->right;
				}
			}
		}
	}
	
}

template <class Key, class Value>
Value& RBTree<Key, Value>::operator[](Key k){
	Value v = 0;
	return insert(k, v)->value;	
}


template <class Key, class Value>
Node<Key, Value>* RBTree<Key, Value>::find(Key k){
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
Node<Key, Value>* RBTree<Key, Value>::left(Node<Key, Value>* n){
	if(n == NULL) return NULL;
	while(n->left != NULL)
		n = n->left;
	return n;
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

/*template <class Key, class Value>
void RBTree<Key, Value>::remove(Key k){
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
	
}*/

template <class Key, class Value>
void printNode(Node<Key, Value>* n){
	if(n != NULL){
		cout << n->key << ": " << n->value;
		if(DEBUG)
			cout << (n->color == BLACK ? " BLACK" : " RED");
		cout << endl;
	}else
		cout << "NULL" << endl;
}

template <class Key, class Value>
void RBTree<Key, Value>::printAux(Node<Key, Value>* n, int h){
	if(n == NULL)
		return;
	int i;
	printAux(n->left, h+1);
	if (DEBUG)
		for(i=0; i<h; i++)
			putchar('\t');
	if(n->value == 0){
		if(DEBUG)
			printNode(n);
	}else
		printNode(n);
	//if(n->value != 0)
	//	cout << n->key << ": " << n->value << endl;
	printAux(n->right, h+1);
}

template <class Key, class Value>
void RBTree<Key, Value>::print(){
	printAux(tree->left, 0);
}

template <class Key, class Value>
void RBTree<Key, Value>::printbfs(){
	queue< Node<string, int> > q;
	queue<int> q2;
	q.push(*(tree->left));
	q2.push(0);
	while(!q.empty()){
		Node<Key, Value> n = q.front();
		printNode(&n);
		printf("(");
			printNode(n.left);
			printNode(n.right);
		printf(") - ");
		printf("%d %s\n", q2.front(), n.color == RED ? "RED" : "BLACK");
		if(n.left != NULL){
			q.push(*(n.left));
			q2.push(q2.front()+1);
		}
		if(n.right != NULL){
			q.push(*(n.right));
			q2.push(q2.front()+1);
		}
		q.pop();
		q2.pop();
		
	}
}

int main(){
	RBTree<string, int> tree;
	
	char rd[50];
	//int c=0;
	while(scanf("%s", rd) != EOF){
		string s (rd);
		//cout << s << endl;
		if(s == "REMOVE"){
			scanf("%s", rd);
			string s2 (rd);
			transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
			tree[s2] = 0;
			//tree.remove(s2);
		}else{
			transform(s.begin(), s.end(), s.begin(), ::tolower);
			tree[s]++;
		}
		
		//tree.print();
		//blackrule(tree.tree->left, 0);
	}
	tree.print();
	//blackrule(tree.tree->left, 0);

	return 0;
}
