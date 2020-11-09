//This is just the default template, you can remove it if you want.

#include <iostream>

using namespace std ;

int main ()
{
 char c[51],sw[51][4];
 int n,temp,al[26],i,j,i1,i2;
 for(i=0;i<26;i++)
  al[i] = i;
 scanf("%s",c);
 scanf("%d",&n);
 for(i=0;i<n;i++)
   scanf("%s",sw[i]);
 //printf("%d",('A'-65));
 for(i=0;i<n;i++)
 {
  i1 = sw[i][0]-65;
  i2 = sw[i][2]-65;
  temp = al[i1];
  al[i1] = al[i2];
  al[i2] = temp;
 }
 //for(i=0;i<26;i++) printf("%d ",al[i]);
 i=0;
 while(c[i]!='\0')
 {
  j=0;
  while((al[j]+65)!=c[i]) j++;
  printf("%c",j+65); 
  i++;
 }
 return 0 ;
}