//This is just the default template, you can remove it if you want.

#include <iostream>

using namespace std ;

int main ()
{
 char arr[50];
 char sw[10][3];
 int num;
 int j;
 char x;
 cin>>arr;
 cin>>num;
 int i;
 for(i=0;i<num;i++)
  {
   cin>>sw[i];
  }
 i=0;
  while(arr[i]!='\0')
   {
    j=0;
    x=arr[i];
    while(j<num)
     {
      if(sw[j][0]==x)
       {
        x=sw[j][2];
        j=0;
       }
      j++;
 
     }
    cout<<x;
    i++;
   }
	return 0 ;
}