#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>

#include<numeric>
#include<set>
int r[9],c[9],b[3][3];

using namespace std;
vector<string>s;
bool recurse(int x,int y)
{

	int i;
	if(x==9)return 1;
	if(y==9){return recurse(x+1,0);}
	if(s[x][y]!='0')return recurse(x,y+1);
	for(i=1;i<=9;i++)
	{
		if((r[x]&(1<<i))==0&&(c[y]&(1<<i))==0&&(b[x/3][y/3]&(1<<i))==0)
		{
			r[x]|=(1<<i);
			c[y]|=(1<<i);
			b[x/3][y/3]|=(1<<i);
			s[x][y]='0'+i;
			if(recurse(x,y+1))return 1;
			r[x]^=(1<<i);
			c[y]^=(1<<i);
			b[x/3][y/3]^=(1<<i);
			s[x][y]='0';
		}
	}
	return 0;
}
		
	
int main()
{
	int i,j;
	for(i=0;i<9;i++)
		r[i]=c[i]=b[i/3][i%3]=0;
	for(i=0;i<9;i++)
	{
		string temp;
		cin>>temp;
		s.push_back(temp);
		for(j=0;j<9;j++)
		{
			r[i]|=(1<<(temp[j]-'0'));
			c[j]|=(1<<(temp[j]-'0'));
			b[i/3][j/3]|=(1<<(temp[j]-'0'));
		}
	}
	if(recurse(0,0)==0)cout<<"No ans";
	for(i=0;i<9;i++)
		cout<<s[i]<<endl;
	return 0;
}
