#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<queue>
#include<numeric>
#include<set>

using namespace std;
#define ABS(a) ((a)<0?-(a):(a))
bool in(string s)
{
	if(s[0]>'h'||s[1]>'8'||s[0]<'a'||s[1]<'1')return 0;
	return 1;
}
int main()
{
	string start,end;
	int memo[8][8];
	int i,j;
	for(i=0;i<8;i++)for(j=0;j<8;j++)memo[i][j]=-1;
	cin>>start>>end;
	queue<string>q;
	q.push(start);
	memo[start[0]-'a'][start[1]-'1']=0;
	int ans;
	while(!q.empty())
	{
		string p=q.front();

		q.pop();
		if(p==end){break;}
		for(i=-2;i<=2;i++)
			for(j=-2;j<=2;j++)
			{
				if(ABS(i)+ABS(j)!=3)continue;
				string temp;
				temp=p;
				temp[0]+=i;
				temp[1]+=j;
				if(in(temp)&&memo[temp[0]-'a'][temp[1]-'1']==-1)
				{
					q.push(temp);
					memo[temp[0]-'a'][temp[1]-'1']=memo[p[0]-'a'][p[1]-'1']+1;
				}
			}
	}

	cout<<"To get from "<<start<<" to "<<end<<" takes "<<memo[end[0]-'a'][end[1]-'1']<<" knight moves.";
	return 0;
}	
	