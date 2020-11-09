#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>

#include<numeric>
#include<set>

using namespace std;
string end;
vector<string>vs;
string pos;
int n;
bool recurse()
{
	if(pos==end)
	{
		vs.push_back(pos);
 		return 1;
	}
	int i;
	bool flag=0;
	for(i=0;i<2*n;i++)
	{
		if(pos[i]=='B'&&pos[i+1]=='_')
		{
			pos[i]='_';
			pos[i+1]='B';
			if(recurse()){flag=1;}
			pos[i]='B';
			pos[i+1]='_';
		}
		else if(pos[i]=='B'&&pos[i+1]=='W'&&i+2<pos.length()&&pos[i+2]=='_')
		{
			pos[i]='_';
			pos[i+2]='B';
			if(recurse()){flag=1;}
			pos[i]='B';
			pos[i+2]='_';
		}
		else if(pos[i+1]=='W'&&pos[i]=='_')
		{
			pos[i]='W';
			pos[i+1]='_';
			if(recurse()){flag=1;}
			pos[i]='_';
			pos[i+1]='W';
		}
		else if(pos[i]=='_'&&pos[i+1]=='B'&&i+2<pos.length()&&pos[i+2]=='W')
		{
			pos[i]='W';
			pos[i+2]='_';
			if(recurse()){flag=1;}
			pos[i]='_';
			pos[i+2]='W';
		}
		if(flag){vs.push_back(pos);return 1;}
	}
	return 0;
}

int main()
{
	int i;
//	n=atoi(argv[1]);
	string s;
cin>>n;
	for(i=0;i<n;i++){s+="B";end+="W";}
	s+="_";
	end+="_";
	for(i=0;i<n;i++){
		s+="W";end+="B";}

	pos=s;
	recurse();	
	for(i=vs.size()-1;i>=0;i--)cout<<vs[i]<<endl;
	
}