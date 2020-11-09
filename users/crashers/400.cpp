//This is just the default template, you can remove it if you want.

#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std ;

int main ()
{
char p[5];
int p11,p12,p21,p22,i,j,k,ans=0;
gets(p);
p11=p[0]-'a'+1;
p12=p[1]-'0';
p21=p[3]-'a'+1;
p22=p[4]-'0';
i=abs(p11-p21);
j=abs(p12-p22);
k=i<j?i:j;
ans=k+abs(i-j);
cout << "To get from "<<p[0]<<p[1]<<" to "<<p[3] <<p[4] << " takes "<<ans<<" knight moves.";
return 0 ;
}