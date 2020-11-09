//This is just the default template, you can remove it if you want.

#include <iostream>
#include<stdio.h>

using namespace std ;

int main ()
{
  char s[7][51];
  int i=0,j=0,cnt=0,k=0;
  int score[9] = {0,1000,2500,5000,10000,25000,50000,75000,125000};
  long int val[8] = {0};
  while(i<7)
  {
   scanf("%s",s[i]);
   i++;
  }
  for(i=0;i<8;i++)
  { 
   j=0;
   while(j<7)
   { 
    k=0;
    cnt=0;
    while(s[j][k]!='\0')
    {
     if(s[j][k]=='W')
      cnt=0;
     if(s[j][k]=='R')
      {cnt++;
     if(cnt==(i+1))
     {
       if(j==6) val[i]+=2*score[i+1];
       else val[i]+=score[i+1];
       cnt=0;
     }}
     k++;
    }
   j++;
  }
 }
  for(i=0;i<8;i++)
  printf("%ld ",val[i]);
	return 0 ;
}