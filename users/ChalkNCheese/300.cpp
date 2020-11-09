#include <iostream>
#include <string>

using namespace std ;
int ans[8];

int main ()
{
 int award[9]={0,1000,2500,5000,10000,25000,50000,75000,125000} ;
 char a[7][51];

 int i,j,cnt,k;
 for(i=0; i<7;i++) cin>>a[i];

 for(i=0;i<7;i++)
 {
 for(k=1;k<=8;k++)
 {
 for(j=0,cnt=0; j<strlen(a[i]); j++) 
 {
  //cout<<a[i][j]<<" ";
  if (a[i][j]=='R') cnt++;
  if (a[i][j]=='W') cnt=0;
  if (cnt==k && i!=6) {ans[k-1]+=award[cnt]; cnt=0;}
  if (cnt==k && i==6) {ans[k-1]+=award[cnt]*2; cnt=0;}
 }
 }
 }

 for (i=0; i<8; i++) cout<<ans[i]<<" ";

 return 0 ;
}