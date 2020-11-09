//This is just the default template, you can remove it if you want.

#include <iostream>

using namespace std ;

int main ()
{

   long int TotalEar;//[8] = {0,0,0,0,0,0,0,0};
   char str[7][50];
   int count;
   long int arr1[9] ={0,1000,2500,5000l,10000l,25000l,50000l,75000l,125000l};
   long int arr2[9] ={0,2000l,5000l,10000l,20000l,50000l,100000l,150000l,250000l};
   
for(int i=0;i<7;i++)
{
  cin>>str[i];
}
//

for(int i=0;i<8;i++)
{
TotalEar=0l;
for(int k=0;k<7;k++)
{
  count=0; 
  for(int j=0;str[k][j]!='\0';j++)
  {
     if(str[k][j]=='R')
     {
			count++;
		}
     else 
		if(str[k][j]=='W')
       count=0;
     if(count==i+1)
		{
      if(k<6)
		 	TotalEar=TotalEar+arr1[count];	
		 else
		 	TotalEar=TotalEar+arr2[count];
      count=0;
		}
  }
}
cout<<TotalEar<<"  ";
}

	return 0 ;
}