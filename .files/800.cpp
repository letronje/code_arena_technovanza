#include <iostream>
#include <set>
#include <fstream>
#include <vector>

using namespace std ;

class Set//Set of integers
{
	public:
		set<int> s;
		void operator += ( const int& x )//adds no to set
		{
			s.insert(x);
		}
		void operator += ( Set& ins )//adds another set to a set
		{
			set<int>::iterator i ;
			for(i=ins.s.begin();i != ins.s.end();i++)
				s.insert(*i);
		}
		void operator -= ( int& x )//removes a no from a set
		{
			set<int> :: iterator i;
			for(i=s.begin();i!=s.end();i++)
			{
				if((*i)==x)
				{
					s.erase(i);
					return ;
				}
			}
		}
};

void GetBoard(Set board[9][9])
{
	Set d;
	for(int i=1;i<=9;i++)
		d += i;
	for(int i=0;i<9;i++)
	{
		string s;
		cin>>s;
		for(int j=0;j<9;j++)
		{
			if(s.at(j)!='0')	
				board[i][j] += s.at(j)-48 ;	
			else
				board[i][j] = d;					
		}
	}
}

void ShowBoard(Set board[9][9])//show board at any instant
{
	cout << endl ;
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			set<int> :: iterator k;
			int ctr=0;
			for(k=board[i][j].s.begin();k!=board[i][j].s.end();k++,ctr++)
				cout << *k ;
		}
		cout << endl ;
	}
}

void ReduceRowCol(Set board[9][9],int& row,int& col)
{
	vector < pair<int,int> > v;
	int ans = *board[row][col].s.begin();
	for(int k=0;k<9;k++)
	{
		if(board[row][k].s.size() > 1)
		{
			board[row][k] -= ans ;
			if(board[row][k].s.size()==1)
				v.push_back(pair<int,int> (row,k));		
		}
		if(board[k][col].s.size() > 1)
		{
			board[k][col] -= ans;
			if(board[k][col].s.size()==1)
				v.push_back(pair<int,int> (k,col));		
		}
	}	
	for(int k=0;k<v.size();k++)
		ReduceRowCol(board,v.at(k).first,v.at(k).second);
}
void ReduceBlock (Set board[9][9],int& row,int& col)
{
	int sr=row/3*3, sc=col/3*3, ans=*board[row][col].s.begin();
	for(int i=sr;i<=(sr+2);i++)
		for(int j=sc;j<=(sc+2);j++)
			if(i!=row && j!=col)
				board[i][j] -= ans;
}

void Copy(Set b1[9][9],Set b2 [9][9])
{
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++)
			b1[i][j]=b2[i][j];
}
void GetSetBackUp(Set b1[9][9],Set b2 [9][9])//copies b2 to b1
{
	Copy(b1,b2);	
}

bool TestRowCol  (Set [9][9],int& ,int& );
bool TestBlock   (Set [9][9],int& ,int& );
bool TestSolution(Set [9][9]);

int no_sol=0;
Set soln [9][9];


int Solve(Set board[9][9],int row,int col)//bactracking solver for higher difficulty
{
	if( col == 9)//if reached 9th col,then Solve next row's 0th col
	{
		col=0;
		row++ ;
		if(row==9)
		{
			if(no_sol==0 )
			{
				if(TestSolution(board))
				{Copy(soln,board);
				no_sol=1;	
				return 1;
				}
			}	
			else
			{
				if(TestSolution(board))
				{
					no_sol ++ ;
				}
			}	
			return 0;
		}
	}		
	
	Set buffer[9][9];
	GetSetBackUp(buffer,board);// take a snapshot of current board
	
	if(board[row][col].s.size()==1)// if a cell is already solved , then solve next col
	{
		ReduceRowCol(board,row,col);
		ReduceBlock(board,row,col);
		return Solve(board,row,col+1);
	}
	
	Set allowed=board[row][col];//set of elements allowed in current cell
	if(allowed.s.empty())
		return 0 ;//dead end
		
	//try each element one by one	
	for(set<int>::iterator i=allowed.s.begin();i!=allowed.s.end();i++)
	{
		board[row][col].s.clear();//clear set of current cell	
		board[row][col].s.insert(*i);//insert allowed value as the single value in the cell
		
		//now reduce no of possible nos in each cell
		
		ReduceRowCol(board,row,col);
		ReduceBlock(board,row,col);
		
		if(Solve(board,row,col+1))
			return 1;
		else
			GetSetBackUp(board,buffer);//backtracking(restore previous state of board from buffer);
	}
	return 0;
}


void CalcPossibilities(Set board [9][9])
{
	//now reduce no of possible nos in each cell
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			if(board[i][j].s.size()==1)
			{
				//remove single no in current cell from all other cells(size>1) in same row & col
				ReduceRowCol(board,i,j);
				//remove single no in current cell from all other cells in same 3x3 block
				ReduceBlock(board,i,j);
			}
		}
	}
}
int main()
{
	Set board[9][9];
	GetBoard(board);
	CalcPossibilities(board);
	Solve(board,0,0);
	if(no_sol==0)
		cout << "Could Not Solve" ;
	else 
		ShowBoard(soln);
	return 0;
}

bool TestRowCol(Set board[9][9],int& row,int& col)
{
	if(board[row][col].s.size()!=1)
	{
		//cout << "\n\nError at ("<<row <<"," << col << ")\n";
		//cout << "\nIncorrect\n\n";
		//exit(1);
		return false;	
	}	
	int already_used = *board[row][col].s.begin();
	for(int k=0;k<9;k++)
	{
		if(k!=col && (*board[row][k].s.begin()) == already_used )
		{
			//cout <<"\n\n("<<row <<"," << k << ") = (" << row << "," << col << ")"<<endl; 
			//cout << "\nIncorrect\n\n";
			//exit(1); 
			return false;	
		}	
		if(k!=row && (*board[k][col].s.begin()) == already_used )
		{
			//cout <<"\n\n("<<k<<"," << col << ") = (" << row << "," << col << ")"<<endl;
			//cout << "\nIncorrect\n\n";
			//exit(1);
			return false;
		}	
	}	
	return true ;
}
bool TestBlock (Set board[9][9],int& row,int& col)
{
	if(board[row][col].s.size()!=1)
	{
		//cout << "\n\nError at ("<<row <<"," << col << ")\n";
		//cout << "\nIncorrect\n\n";
		//exit(1) ;
		return false;
	}
	int sr=row/3*3;
	int sc=col/3*3;
	
	int ans=*board[row][col].s.begin();
	
	for(int i=sr;i<=(sr+2);i++)
	{
		for(int j=sc;j<=(sc+2);j++)
		{
			if(i!=row && j!=col && ((*board[i][j].s.begin()) == ans))
			{
				//cout <<"\n\n("<<i<<"," << j << ") = (" << row << "," << col << ")"<<endl;
				//cout << "\nIncorrect\n\n";
				//exit(1);	
				return false;
			}	
		}
	}
	return true ;
}

bool TestSolution(Set board[9][9])
{
	//cout << "Testing ... " ;
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			if(!TestRowCol(board,i,j))
				return false;
			if(!TestBlock(board,i,j))
				return false;
		}
	}
	//cout << "Correct\n\n";
	return true ;
}

/*

0 5 0 0 0 3 0 0 0 
8 0 0 2 0 0 6 0 4
0 0 0 1 5 0 0 0 0 
1 0 0 9 8 0 0 0 6
6 0 4 0 0 0 8 0 5
9 0 0 0 6 2 0 0 3
0 0 0 0 9 1 0 0 0 
2 0 8 0 0 6 0 0 1
0 0 0 5 0 0 0 7 0

ans = 2

7 5 2 6 4 3 1 8 9
8 1 3 2 7 9 6 5 4
4 6 9 1 5 8 2 3 7
1 3 7 9 8 5 4 2 6
6 2 4 3 1 7 8 9 5
9 8 5 4 6 2 7 1 3
5 7 6 8 9 1 3 4 2
2 9 8 7 3 6 5 4 1
3 1 6 5 2 4 9 7 8

7 5 2 6 4 3 1 8 9
8 3 1 2 7 9 6 5 4
4 6 9 1 5 8 2 3 7
1 7 3 9 8 5 4 2 6
6 2 4 3 1 7 8 9 5
9 8 5 4 6 2 7 1 3
5 4 7 8 9 1 3 6 2
2 9 8 7 3 6 5 4 1
3 1 6 5 2 4 9 7 8

*/