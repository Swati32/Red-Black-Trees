
//

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct node
{
	int key;
	int count;
	node *parent;
	char color;
	node *left;
	node *right;
};

class rbtree
{
	node *root;
	node *q;
    public:
	rbtree()
	{
		q = NULL;
		root = NULL;
	}

	void insert(int id, int count);
	void complete_insertion(node *);
	void leftrotate(node *);
	void rightrotate(node *);
	void del(node *p);
	void complete_deletion(node *);
	void next(node *root ,int id,int &set);
	void inrange(node *root, int id1,int id2,int* sum);
	void  previous(node *root, int id, int *set  ,int* pkey,int* pcount);
	void reduce(int id, int m);
	void increase(int id, int m);
	node* successor(node *);
	node* createExternalNode(node *);
	node* search(int id);
	node* getroot();
	
};

//Function to find previous nodes of given id  

void rbtree::previous(node* root, int id, int* set,int* pkey,int* pcount)
{
	if (root->key != -1)
	{
		
                previous(root->left, id, set ,pkey,pcount);
               
		if (root->key > id && *set != 2 && *pkey == -1) {           // If given id is the first id , there are no previous nodes
		       cout<<0<<" "<<0<<endl;
                	*set = 2;
		}

		if (root->key >= id && *set  !=2) {                         // If root is the given id  
			cout << *pkey << " " << *pcount << endl;
			*set = 2;
		}

		if (*pkey != root->right->key){                             // Set root as previous node before moving to next node
			*pkey=root->key;
                        *pcount =root->count;
                }
		previous(root->right, id, set ,pkey,pcount);
		
	}
}

//Function to find next nodes of given id  

void rbtree::next(node* root,int id,int &set)
{ 
	if (root->key != -1)              
	{
	
        	next(root->left,id,set);
                if (root->key > id && set  !=2) {                           // If root is the given id
                        cout << root->key << " " << root->count << endl;
                        set = 2;
                }
                                             
		next(root->right, id, set);
	}
}

//Function to find sum of sum of integers between range id1 and id2

void rbtree::inrange(node* root, int id1,int id2,int* sum)
{
	if (root->key != -1)
	{
		inrange(root->left, id1,id2,sum);                       
		if (root->key>= id1 && root->key <=id2) {                  // If root is within given range add count to total sum
			*sum += root->count;
		}
		inrange(root->right, id1, id2, sum);
	}
}


//Function to search an element 

node* rbtree::search(int id) {
	if (root == NULL)
		return NULL;
	node *p = root;
	while (p->key != -1) {                                             // If not external node
		if (p->key == id)                                          // Element found return node pointer of the element
			return p;
		if (p->key < id)
			p = p->right;
		if (p->key > id)
			p = p->left;
       }
return NULL;
}

//Function to create external nodes and initialize it as color black and key as -1

struct node* rbtree::createExternalNode(node * p) {
	node *q = new node;
	q->parent = p;
	q->color = 'b';
	q->key = -1;
	q->count = NULL;
	q->left = NULL;
	q->right = NULL;
	return q;
}

//Function to insert an element into the red black tree

void rbtree::insert(int id, int count)
{
	int i = 0;

	node *p, *q;
	
	node *t = new node;                                                 // Create New node element and initialize it.
	t->key = id;
	t->count = count;
	t->left = createExternalNode(t);
	t->right = createExternalNode(t);
	t->color = 'r';
	p = root;
	q = NULL;

	if (root == NULL)                                                   // New element is the root
	{
		root = t;
		t->parent = NULL;
	}
	else
	{
		while (p->key != -1)                                        // Traverse the tree to fins the right position
		{
			q = p;
			if (p->key<t->key)
				p = p->right;
			else
				p = p->left;
		}

		t->parent = q;                                              // Make q as the parent of new node
		
		if (q->key<t->key)                                          // Make t child of new node
			q->right = t;
		else
			q->left = t;

	}
	complete_insertion(t);                                              // Complete insertion by assigning right color
}


// Function to readjust colors of nodes so that all the contraints of reb black satisfied

void rbtree::complete_insertion(node *t)
{
	node *u;
	if (root == t)
	{
		t->color = 'b';
		return;
	}
	while (t->parent != NULL && t->parent->color == 'r')                         // Loop until no red red violations exist
	{
		node *g = t->parent->parent;
		if (g->left != NULL && g->left == t->parent)
		{
			u = g->right; 
			if (u->color == 'r')                                        // Uncle is red LXr case ,
			{                                                           // Change the color of the parent to black
				t->parent->color = 'b';                             // grandparent to red and repeat on grandparent
				u->color = 'b';
				g->color = 'r';
				t = g;
			}
			else {                                                      // Uncle is black Two cases Left Right/Left Left
				if (t->parent->left == t) {                         // left left black case
					rightrotate(g);
					char temp = g->color;
					g->color = t->parent->color;
					t->parent->color = temp;
				}
				else {                                              // left right black base
					t = t->parent;
					leftrotate(t);
					rightrotate(g);
					char temp = g->color;
					g->color = t->parent->color;
					t->parent->color = temp;
				}
			}

		}
		else if (g->right != NULL && g->right == t->parent) {                 // parent is on right side
			u = g->left;                                                  // Uncle is on left side
			if (u->color == 'r')                                          // RXr case ,Change the color of parent to black,
			{                                                             // grandparent to red and repeat on grandparent
				t->parent->color = 'b';
				u->color = 'b';
				g->color = 'r';
				t = g;
			}
			else {                                                        // Uncle is black
				if (t->parent->right == t) {                          // right right case
					leftrotate(g);
					char temp = g->color;
					g->color = t->parent->color;
					t->parent->color = temp;
				}
				else {                                                // right left case  
					t = t->parent;
					rightrotate(t);
					leftrotate(g);
					char temp = g->color;
					g->color = t->parent->color;
					t->parent->color = temp;
				}                                                        
			}
			root->color = 'b';
		}
	}
}

//Function to delete a node from the tree

void rbtree::del(node *p)
{
	if (root == NULL)
	{
		cout << "\nEmpty Tree.";
		return;
	}
	
	
	node *y = NULL;
	node *q = NULL;
	
	if (p->left != NULL || p->right != NULL)                        // If node to be deleted is external node 
		y = p;
	else
	    y = successor(p);               
         

	if (y->left != NULL)                                            // Get the child node                                             
		q = y->left;
	else
	{
		if (y->right->key != -1)
			q = y->right;
		else
			q = NULL;
	}
	if (q != NULL)                                                  // If child exist , make grandparent the parent of child 
		q->parent = y->parent;
		
	if (y->parent == NULL)                                          // If node to be removed was root node , make child the root node
			root = q;
	else
		{
			if (y == y->parent->left)                       // If removed node was left child of its parent  
				y->parent->left = q;                    // make the child of removed node left child of its grandparent
			else                                            // and vice versa
				y->parent->right = q;
		}
	if (y != p)                                                            
	{
		p->color = y->color;
		p->key = y->key;
	}
		
	if (y->color == 'b')                                             // If the color of replacing node is black 
		complete_deletion(q);                                    // number of black nodes from root to leaf might change
	                                                                 // call complete insertion to fix it
}


// Function to fix colors of nodes when a node is removed such that no red black tree violations exist

void rbtree::complete_deletion(node *p)
{
	node *s;
	while (p != root && p->color == 'b')                                  
	{
		if (p->parent->left == p)
		{
			s = p->parent->right;
			if (s->color == 'r')
			{
				s->color = 'b';
				p->parent->color = 'r';
				leftrotate(p->parent);
				s = p->parent->right;
			}
			if (s->right->color == 'b'&&s->left->color == 'b')
			{
				s->color = 'r';
				p = p->parent;
			}
			else
			{
				if (s->right->color == 'b')
				{
					s->left->color = 'b';
					s->color = 'r';
					rightrotate(s);
					s = p->parent->right;
				}
				s->color = p->parent->color;
				p->parent->color = 'b';
				s->right->color = 'b';
				leftrotate(p->parent);
				p = root;
			}
		}
		else
		{
			s = p->parent->left;
			if (s->color == 'r')
			{
				s->color = 'b';
				p->parent->color = 'r';
				rightrotate(p->parent);
				s = p->parent->left;
			}
			if (s->left->color == 'b'&&s->right->color == 'b')
			{
				s->color = 'r';
				p = p->parent;
			}
			else
			{
				if (s->left->color == 'b')
				{
					s->right->color = 'b';
					s->color = 'r';
					leftrotate(s);
					s = p->parent->left;
				}
				s->color = p->parent->color;
				p->parent->color = 'b';
				s->left->color = 'b';
				rightrotate(p->parent);
				p = root;
			}
		}
		p->color = 'b';
		root->color = 'b';
	}
}

//Function to left rotate tree about pivot

void rbtree::leftrotate(node *p)
{
	if (p->right == NULL)
		return;
	else
	{
		node *y = p->right;
		
		if (y->left != NULL)
		{
			p->right = y->left;
			y->left->parent = p;
		}
		else
			p->right = NULL;
		
		if (p->parent != NULL)
			y->parent = p->parent;
		
		if (p->parent == NULL)
			root = y;
		else
		{
			if (p == p->parent->left)
				p->parent->left = y;
			else
				p->parent->right = y;
		}
		y->left = p;
		p->parent = y;
	}
	root->parent = NULL;
}

//Function to right rotate tree about pivot

void rbtree::rightrotate(node *p)
{
	if (p->left == NULL)
		return;
	else
	{
		node *y = p->left;
		
		if (y->right != NULL)
		{
			p->left = y->right;
			y->right->parent = p;
		}
		else
			p->left = NULL;
		
		if (p->parent != NULL)
			y->parent = p->parent;
		
		if (p->parent == NULL)
			root = y;
		else
		{
			if (p == p->parent->left)
				p->parent->left = y;
			else
				p->parent->right = y;
		}
		y->right = p;
		p->parent = y;
	}
	root->parent = NULL;
}

// Function to get successor node i.e node which will be replacing node to be removed

node* rbtree::successor(node *p)
{
	node *y = NULL;
	if (p->left->key != -1)
	{
		y = p->left;
		while (y->right->key != -1)
			y = y->right;
	}
	else
	{
		y = p->right;
		while (y->left->key != -1)
			y = y->left;
	}
	return y;
}


// Function to get root of the tree

node * rbtree::getroot()
{
	return root;
}

// Function to split string on delimiter

vector<string> split(string s, char delim) {
	stringstream ss(s);
	string item;
	vector<string> arg;
	while (getline(ss, item, delim)) {
		arg.push_back(item);
	}
	return arg;
}

//Function to increase count of given id 

void rbtree::increase(int id, int m) {
	node *p = search(id);                                                // Find node
	if (p == NULL) {                                                     // Insert if not found
		insert(id, m);
		cout << m<<endl;
	}
	else {
		p->count += m;                                               // Increase count by m if found
		cout << p->count<<endl;
	}
}


//Function to reduce count of given id

void rbtree::reduce(int id, int m) {
	node *p = search(id);                                                // Find element 
	if (p == NULL) {
		cout << 0<<endl;
	}
	else {                                                               // Reduce count by m
		p->count = p->count - m ;
		if (p->count <= 0) {                                         // If count is less than zero after reduction ,delete node
			cout << 0<<endl;
			del(p);
		}
		else
			cout << p->count<<endl;
	}
}

// Interactive commands


void run(rbtree t) {
	string command;
	getline(cin, command);
	vector<string> arg = split(command, ' ');

	if (arg[0].compare("increase") == 0) {
		t.increase(atoi(arg[1].c_str()), atoi(arg[2].c_str()));
		
	}
	else if (arg[0].compare("reduce") == 0) {
		t.reduce(atoi(arg[1].c_str()), atoi(arg[2].c_str()));
		
	}
	else if (arg[0].compare("count") == 0) {
	
	    node *p = t.search(atoi(arg[1].c_str()));
		if (p == NULL)
			cout << 0 << endl;
		else
			cout << p->count << endl;
	
	}else if (arg[0].compare("inrange") == 0) {
	
	   int sum = 0;
	   int set1,set2 = 0;
	   t.inrange(t.getroot(), atoi(arg[1].c_str()), atoi(arg[2].c_str()),&sum);
	 
		   cout << sum << endl;
	}
	else if (arg[0].compare("next") == 0) {
		
		int set = 0;
		t.next(t.getroot(),atoi(arg[1].c_str()),set);
		if (set != 2)
			cout << 0 << " " << 0 <<endl;
		
	}
	else if (arg[0].compare("previous") == 0) {
		
		int set=0;
                int pcount = 0;
                int pkey=-1;
		t.previous(t.getroot(),atoi(arg[1].c_str()),&set,&pkey,&pcount);
		if (set==0)
                cout<<pkey<<" "<<pcount<<endl;
	
	}
	else if (arg[0].compare("quit") == 0) {
		return;
	}
	else {
		cout << "Invalid command" << endl;
	}
	
	run(t);
}


int main(int argc,char * argv[])
{
	
	rbtree obj;
    const char* s= argv[1];
	string line;
	ifstream file;
	file.open(s);

	// Read file and create the tree
	cout<<"filename"<<s;
	if (file.is_open()) {
		getline(file, line);
		int n = atoi(line.c_str());
                                
		while (n > 0)
		{
			getline(file, line);
        	vector<string> data = split(line, ' ');
        	int id = atoi(data[0].c_str());
			int count = atoi(data[1].c_str());
        	obj.insert(id, count);
			n--;

		}
  
	    file.close();
	}
			
	run(obj);
	
 return 1;

}
