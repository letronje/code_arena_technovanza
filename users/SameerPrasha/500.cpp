//This is just the default template, you can remove it if you want.

#include <iostream>

using namespace std ;

int main ()
{
  char dic[50][30];
  int flag[50];  
  for(int i=0;i<50;i++) 
    flag[i]=0;
  int num;
  cin>>num;
  int i;
  for(i=0;i<num;i++)
   {
    cin>>dic[i];
   }
  char mess[50];
  cin>>mess;
  char op[100];
  char temp[50];

  i=0;
  char str[30];
  int pos=0;
  int j=0;
  int po=0;
  int se=0;
  int kj=0;
  int l;
  char Result[100];
  int q,p,y;
  while(1)
   {
    se=0; 
    pos=kj;
    if(mess[pos]=='\0')
      break;
    for(int gg=0;str[gg]!='\0';gg++)
       str[gg]='\0';
  while(mess[i]!='\0')
   {
    str[pos]=mess[j];
    l=0;
    while(l==0)
     {
      for(p=0;p<num;p++)
       {
 			 y=0;	
			 for(q=0;dic[p][q]!='\0';q++)
        	{
  					if(str[y]=='\0')
 						break;
 					if(str[y]!=dic[p][q])
						break;
					y++;	
				}
         if(dic[p][q]=='\0' && str[y]=='\0')
          {
            
					if(se==1)
                cout<<"Ambigous";
					for(int r=0;str[r]!='\0';r++)
              {
						 	
   					Result[po]=str[r];
						po++;
 					 }
            kj=pos;
            Result[po]=' '; 
					po++;
            l=1;
					se=1;
				}
			}
     }   
    i++;
   }

    j++;
}
  cout<<Result;
/*  i=0;
  while(mes[i]!='\0')
  {
    x=mess[i];
    for(k=0;k<num;k++)
     {
       if(dic[k][j]!='\0'  && dic[k][j]==x)
        flag[k]=1;
       else
        flag[k]=0;
     }
    i++;
  }*/
  
	return 0 ;
}