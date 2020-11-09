//This is just the default template, you can remove it if you want.

#include <iostream>

using namespace std ;

int main ()
{
int ans[8] = {0},i,j,k,count[8]={0};
int award[] = {0, 1000, 2500, 5000, 10000, 25000, 50000, 75000, 125000 };
char inp[7][50];
for(i=0;i<7;i++)
	cin >> inp[i];
for(i=0;i<7;i++)
{
for(j=0;inp[i][j]!='\0';j++)
{
	if(inp[i][j] == 'R')
		for(k=0;k<8;k++)
		{count[k]++;
		if(count[k]%(k+1) == 0)
		ans[k]+=award[k];
		}
	if(inp[i][j] == 'W')
		for(k=0;k<8;k++)
		count[k]=0;
}
}
for(i=0;i<8;i++)
	cout << ans[i] << ' ';
return 0 ;
}