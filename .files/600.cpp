#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define BLACK 	'B'
#define WHITE	'W'
#define EMPTY	'_'

using namespace std ;

int no_frogs;//no of frogs per side
string final;
vector<string> soln;

struct Node
{
	string s;
	Node* left;
	Node* right;	
	Node* father ;
};
void Swap(string& s,int i,int j)
{
	int temp=s.at(i);
	s.at(i)=s.at(j);
	s.at(j)=temp ;
}
void ShowSol(vector<string>& v)
{
	for(int i=0;i<v.size();i++)
		cout << v.at(i)<<endl;	
}
Node* AddAsChild(Node& n,int i,int j)
{
	string temp=n.s;
	Swap(temp,i,j);
	Node** child;
	if(n.left == NULL)
		child=&(n.left);
	if(n.right == NULL)
		child=&(n.right);
		
	*child = new Node;
	(*child)->s = temp;
	(*child)->left=NULL;
	(*child)->right=NULL ;
	(*child)->father=&n;
	
	if((*child)->s == final)
	{
		soln.clear();
		Node* temp =  (*child)->father;
		soln.push_back(final);	

		while(temp !=NULL)
		{
			soln.push_back(temp->s);
			temp = temp->father;
		}
		reverse(soln.begin(),soln.end());
		ShowSol(soln);
		exit(0);
	}
	else
		return (*child);
}

void MakeTree(Node& n)
{
	int i=0;
	while(i<n.s.length() && n.s.at(i)!=EMPTY )
		i++;
	int ll=i-2,ul=i+2;
	if(ll < 0 )
		ll=0;
	if(ul >(n.s.length()-1))
		ul=n.s.length()-1;
	for(int j=ll;j<=ul;j++)
	{
		if(n.s.at(j)==BLACK )
		{
			if(j+1<=ul && n.s.at(j+1)==EMPTY)
				MakeTree(*AddAsChild(n,j,j+1));	
			if(j+2<=ul && n.s.at(j+2)==EMPTY && n.s.at(j+1) == WHITE)
				MakeTree(*AddAsChild(n,j,j+2));
		}	
		else if ( n.s.at(j)==WHITE )	
		{
			
			if(j-1>=ll && n.s.at(j-1)==EMPTY )
				MakeTree(*AddAsChild(n,j,j-1));
			if(j-2>=ll && n.s.at(j-2)==EMPTY && n.s.at(j-1) == BLACK)
				MakeTree(*AddAsChild(n,j-2,j));
		}	
	}
}

int main (int argc,char* argv[])
{
	
	no_frogs=atoi(argv[1]) ;
	if(no_frogs>20)
		return 0;
	Node n;
	int i=0;
	for(i=0;i<no_frogs;i++)
		n.s += BLACK;	
	n.s += EMPTY;
	
	for(i=0;i<no_frogs;i++)
		n.s += WHITE;	
	final=n.s;
	reverse(final.begin(),final.end());
	n.left=n.right=NULL;
	n.father=NULL ;
	MakeTree(n);
	return 0;
}