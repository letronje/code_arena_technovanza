//This is just the default template, you can remove it if you want.

#include <iostream>

using namespace std ;

int main ()
{
 char dict[50][51],msg[51],res[100];
 static int i,j,k,l,n,flag=0,st;
 scanf("%d",&n);
 for(i=0;i<n;i++)
  scanf("%s",dict[i]);
 scanf("%s",msg);
 while(flag<2)
 {
  k=0;
 while(msg[k]!='\0')
 {
 if(k==0) i=st;
 else i=0;
 while(i<n)
 {
  j=0;
  while(dict[i][j]!='\0')
  { 
   if(dict[i][j]!=msg[k]) break;
   if(dict[i][j]==msg[k])
    {
     j++;
     res[l++]=msg[k++];
    }
   st = i;
  }
  res[l++]=' ';
  i++;
 }
 res[l]='\0';
 flag++;
} }
 switch(flag)
 {
 case 0 : printf("IMPOSSIBLE!");
          break;
 case 1 : printf("%s",res);
          break;
 case 2 : printf("AMBIGOUS");
}

	return 0 ;
}