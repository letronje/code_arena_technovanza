#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<numeric>
#include<set>

using namespace std;
int award[] = { 0, 1000, 2500, 5000, 10000, 25000, 50000, 75000, 125000 } ;
int main()
{
	string s;
	int i,j,k;
	vector<int>v(8,0);
	for(i=0;i<7;i++)
	{
		cin>>s;
		for(j=1;j<=8;j++)
		{
			int count=0;
			for(k=0;k<s.length();k++)
			{
				if(s[k]=='W')count=0;
				else count++;
				if(count==j)
				{
					v[j-1]+=(i==6?2*award[j]:award[j]);
					count=0;
				}
			}
		}
	}
	
	cout<<v[0];
	for(i=1;i<8;i++)cout<<" "<<v[i];
	return 0;
}