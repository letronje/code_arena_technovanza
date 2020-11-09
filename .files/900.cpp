#include <iostream>
#include <vector>

#define EMPTY '0'
#define SOLID '1'

using namespace std ;

// a piece can be placed on a board in many possible ways(,i.e,horiz_possibs*vert_possibs)
// in each way , corner represents (row,col) of top,left corner of piece
// scoords => vector of all such corners. (when a piece is placed 
//                                 in each of its possible positions one by one)
typedef pair<int,int> corner ;
typedef vector<corner> corners ; 

typedef vector<string> piece ; //each row=string
typedef vector<string> board ;

vector < piece > pieces ;
vector < corners > pieces_corners ;
vector < board > solns ;

int sqr_size;//size of square (to makes things tough 
						//we can ask them to calc size of square through code)

void GetPieces(const int& no_pieces)
{
	for(int i=0;i<no_pieces;i++)
	{
		int no_rows,no_cols ;
		cin>>no_rows>>no_cols ;
		piece p ;
		for(int j=0;j<no_rows;j++)
		{
			string s;
			cin>>s;
			p.push_back(s);
		}
		pieces.push_back(p);
	}
}

corners CalcPlacements(const int& pindex )//return corners of piece with index 'pindex'
{
	int horiz_possibs = sqr_size-pieces.at(pindex).size()+1;
	int vert_possibs = sqr_size-pieces.at(pindex).at(0).size()+1;//.at(0,1,..any) is ok

	corners starts ; 

	for(int i=0;i<horiz_possibs;i++)
	{
		for(int j=0;j<vert_possibs;j++)
		{
			corner c;
			c.first=i;
			c.second=j;
			starts.push_back(c);		
		}
	}
	return starts ;
}

//returns false if piece at pindex cannot be placed on the board without overlapping.
//return true if otherwise & also changes the board accordingly
bool Place(board& b,const int& pindex,const corner& start)
{
	char piece_char = pindex+49 ;//1st board's char ='1',2nd board's char ='2',...
	board tempb =b;//backup
	piece p = pieces.at(pindex);
	int prows = p.size();//no of rows occupied by piece
	int pcols = p.at(0).length();//no of cols occupied by piece
	for( int i=start.first;i<start.first+prows;i++)
	{
		for(int j=start.second;j<start.second+pcols;j++)
		{
			char c = p.at(i-start.first).at(j-start.second) ;
			if(tempb.at(i).at(j)==SOLID && c ==SOLID)//if there is overlapping
				return false;
			else if(c == SOLID)//if ( no overlapping & if pos is SOLID )
				tempb.at(i).at(j)= piece_char ;//as mentioned in problem
											   //to distinguish between diff pieces in 
											   // final soln.	
		}
	}
	b=tempb;//apply changes if placement successfull
	return true ;
}
void ShowBoard(const board& b)
{
	for(int i=0;i<sqr_size;i++)
		cout << b.at(i) << endl ;
	cout << endl ;
}
bool IsCompletelyFilledSquare(const board& b)//is Board completely solid
{
	for(int i=0;i<b.size();i++)
		if(b.at(i).find(EMPTY)!=-1)
			return false;
	return true ;	
}
void Solve(board& b,const int& pindex )//backtracking solver(solves piece by piece0
{
	if(pindex==pieces.size())//if all pieces have been used
	{
		if(IsCompletelyFilledSquare(b))
		{
			//ShowBoard(b);
			solns.push_back(b);
		}	
		return ;
	}
	board backup=b;
	corners s = pieces_corners.at(pindex);
	for(int i=0;i<s.size();i++)
	{
		if(Place(b,pindex,s.at(i)))//if placement was successful
			Solve(b,pindex+1);
		b=backup ;//always apply backup to find all solns
	}
}
int main ()
{
	int no_pieces ;
	cin>>no_pieces>>sqr_size ;
	GetPieces(no_pieces);
	pieces_corners.clear();
	for(int i=0;i<no_pieces;i++)
		pieces_corners.push_back(CalcPlacements(i));
	board b;
	string zeroes(sqr_size,'0');
	
	for(int i=0;i<sqr_size;i++)
		b.push_back(zeroes);
	solns.clear();
	Solve(b,0);
	if(solns.size()==0)
	{	
		cout << "Impossible";
		return 0;
	}
	else if(solns.size() ==1)
	{
		ShowBoard(solns.at(0));
		return 0;
	}	
	else if(solns.size() > 1 )
	{
		cout << solns.size();
		return 0;
	}	
	return 0;
}