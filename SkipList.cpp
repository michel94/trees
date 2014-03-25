#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <vector>

using namespace std;

string _TABS;

template <class Key, class Value > 
class Node{
	public:
		Node(int l);
		Node(Key k, Value v, int l);

		vector<Node<Key, Value>*> next;
		Key key;
		Value value;
		int l;
};

template <class Key, class Value>
Node<Key, Value>::Node(int l){
	next.resize(l);
}

template <class Key, class Value>
Node<Key, Value>::Node(Key k, Value v, int l){
	key = k;
	value = v;
	next.resize(l);
}

template <class Key, class Value>
class SkipList{
	public:
		SkipList(int );
		Node<Key, Value>* insert(Key, Value);
		Node<Key, Value>* find(Key);
		void printList();
		void remove(Key);
		void print() const;
		void printDebug() const;
		Value& operator [] (Key k);
	private:
		int getLevel();
		Node<Key, Value>* header;
		Node<Key, Value>* findPath(Key k, vector<Node<Key, Value>* >& cur, vector<Node<Key, Value>* >& back);
		int levels;

};

template <class Key, class Value>
SkipList<Key, Value>::SkipList(int l){
	levels = l;

	header = new Node<Key, Value>(levels);
	for(int i=0; i<levels; i++){
		header->next.push_back(NULL);
	}

}

int randNum(int n){
	return rand() % n + 1;
}

template <class Key, class Value>
int SkipList<Key, Value>::getLevel(){
	int r = randNum((1 << levels) - 1);

	int t = levels - 1 - (int)log2((double)r);

	return t;
}

template <class Key, class Value>
Node<Key, Value>* SkipList<Key, Value>::findPath(Key k, vector<Node<Key, Value>* >& cur, vector<Node<Key, Value>* >& back){
	back[levels-1] = header;
	cur[levels-1] = header->next[levels-1];

	for(int lvl = levels-1; lvl>=0; lvl--){
		while(cur[lvl] != NULL && cur[lvl]->key < k){
			back[lvl] = cur[lvl];
			cur[lvl] = cur[lvl]->next[lvl];
		}
		
		if(lvl > 0){
			if(back[lvl] == NULL){
				cur[lvl-1] = header->next[lvl-1];
				back[lvl-1] = NULL;
			}else{
				cur[lvl-1] = back[lvl]->next[lvl-1];
				back[lvl-1] = back[lvl];
			}
		}

	}

	if(cur[0] == NULL)
		return NULL;
	if(cur[0] -> key == k)
		return cur[0];
	
	return NULL;

}

template <class Key, class Value>
Node<Key, Value>* SkipList<Key, Value>::insert(Key k, Value v){
	vector<Node<Key, Value>* > cur;
	vector<Node<Key, Value>* > back;
	cur.resize(levels); back.resize(levels);
	
	back[levels-1] = NULL;
	cur[levels-1] = header->next[levels-1];

	for(int lvl = levels-1; lvl>=0; lvl--){
		while(cur[lvl] != NULL && cur[lvl]->key < k){
			back[lvl] = cur[lvl];
			cur[lvl] = cur[lvl]->next[lvl];
		}
		if(cur[lvl] != NULL && cur[lvl]->key == k)
			return cur[lvl];
		
		if(lvl > 0){
			if(back[lvl] == NULL){
				cur[lvl-1] = header->next[lvl-1];
				back[lvl-1] = NULL;
			}else{
				cur[lvl-1] = back[lvl]->next[lvl-1];
				back[lvl-1] = back[lvl];
			}
		}

	}
	
	int level = getLevel();

	Node<Key, Value>* n = new Node<Key, Value>(k, v, levels);
	n->l = level+1;
	for(int i=0; i<=level; i++){
		n->next[i] = cur[i];
		if(back[i] == NULL)
			header->next[i] = n;
		else
			back[i]->next[i] = n;
	}
	for(int i=level+1; i<levels; i++){
		n->next[i] = NULL;
	}

	return n;
}

template <class Key, class Value>
Value& SkipList<Key, Value>::operator[](Key k){
	Value v = 0;
	return insert(k, v)->value;
}

template <class Key, class Value>
Node<Key, Value>* SkipList<Key, Value>::find(Key k){
	vector<Node<Key, Value>* > cur;
	vector<Node<Key, Value>* > back;
	cur.resize(levels); back.resize(levels);
	
	return findPath(k, cur, back);	
}

template <class Key, class Value>
void SkipList<Key, Value>::remove(Key k){
	vector<Node<Key, Value>* > cur;
	vector<Node<Key, Value>* > back;
	cur.resize(levels); back.resize(levels);
	
	Node<Key, Value>* n = findPath(k, cur, back);

	if(n != NULL){
		for(int i=levels-1; i>=0; i--){
			if(back[i] != NULL){
				if(back[i]->next[i] == n)
					back[i]->next[i] = n->next[i];
			}
			
		}
		delete n;
	}
	
}

template <class Key, class Value>
void SkipList<Key, Value>::print() const{
	for(Node<Key, Value>* n=header->next[0]; n!=NULL; n=n->next[0]){
		if(n->value != 0)
			cout << n->key << ": " << n->value << endl;
	}
}

template <class Key, class Value>
void SkipList<Key, Value>::printDebug() const{
	for(Node<Key, Value>* n=header->next[0]; n!=NULL; n=n->next[0]){
		//if(n->value != 0)
			cout << n->key << ": " << n->value << " links: " << n->l << endl;
		for(int i=0; n->next[i] != NULL; i++){
			cout << "\t" << n->next[i]->key << ": " << n->next[i]->value << endl;
		}
	}
}

template <class Key, class Value>
ostream& operator<< (ostream & out, const SkipList<Key, Value>& t){
	t.print();
	return out;
}

template <class Key, class Value>
void SkipList<Key, Value>::printList(){
	string matrix[11][100];
	int i;
	Node<Key, Value>* n = header->next[0];
	int in;
	for(i=levels-1; i>=0; i--){
		if(header->next[i] != NULL)
			matrix[i][0] = header->next[i]->key;
		else
			matrix[i][0] = "";
	}
	for(in=1; n!=NULL; n=n->next[0], in++){
		printf("%d\n", n->l);
		for(i=levels-1; i>=0; i--){

			if(n->next[i] != NULL)
				matrix[i][in] = n->next[i]->key;
			else
				matrix[i][in] = "";
		}
	}
	int s = in;
	for(i=levels-1; i>=0; i--){
		for(int in=0; in<s;in++){
			cout << matrix[i][in] << "\t";
		}
		cout << endl;
	}
	cout << endl;
	cout << "\t";
	for(n=header->next[0]; n!=NULL; n=n->next[0]){
		cout << n->key << "\t";
	}
	cout << endl;

}

int main(){
	srand (time(NULL));

	SkipList<string, int> list (10);
	
	char rd[50];
	int n = 0;
	while(scanf("%s", rd) != EOF){
		n++;
		
		string s (rd);
		if(s == "REMOVE"){
			scanf("%s", rd);
			string s2 (rd);
			transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
			list.remove(s2);
			
		}else{
			transform(s.begin(), s.end(), s.begin(), ::tolower);
			list[s]++;
		}
	}
	
	list.print();

	return 0;
}
