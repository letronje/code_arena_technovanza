//This is just the default template, you can remove it if you want.

#include <iostream>

using namespace std ;

int main ()
{
	int k,i,j;
	char code[50];
	char decode[50];
	char combi[50][3],keys[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  
	char ch;
	int flag,ftime;

	scanf("%s",code);
 
	scanf("%d",&k);
	for(i=0;i<k;i++)
	{ scanf("%s",combi[i]);
    ch=keys[combi[i][0]-'A'];
    keys[combi[i][0]-'A']=combi[i][2];
    keys[combi[i][2]-'A']=ch;  
  }

  for (i=0; code[i]!='\0'; i++)
 {
   for (j=0; keys[j]!='\0'; j++)
      if (code[i]==keys[j])
         break;
     
   decode[i]=j+'A';
}
decode[i]='\0';

/*
for(i=0;code[i]!='\0';i++)
{
 ch=code[i];
 flag=0;ftime=0;
 for(int j=0;j<k;j++)
 {
	if(combi[j][0]==ch)
		{ if(ftime==1) { ch=combi[j][2];  ftime=1; decode[i]=combi[j][2]; flag=1; } }
 }
 if(flag==0) decode[i]=ch;
}
decode[++i]='\0';*/



	printf("%s",decode);
	return 0 ;
}