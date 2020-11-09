//This is just the default template, you can remove it if you want.

#include <iostream>
#include <string.h>
#include <ctype.h>

using namespace std ;

int main ()
{
//printf(" k ");

int n,i,j,k,l,amg=0,wer=0,xxx=0;
scanf("%d",&n);
char a[n][100],str[100],a1[100],a2[100];
//printf(" k ");
for(i=0;i<n;i++)
{
scanf("%s",&a[i]);
}
scanf("%s",str);


for(i=0;i<n;i++)
{
if(!strcmp(a[i],str))
{printf("%s",str);
wer=1;}
}

if(!wer)
{
for(i=0;i<n;i++)
{
if(amg)break;

 for(j=0;j<strlen(a[i]);i++)
	{ 
if(amg)break;

		for(k=0;k<n;k++)
		{
int erer=0;
if( i == k )
 {
//printf(" j");
 continue;erer=1;
}

//printf("l ");

if(amg)break;
			if(a[i][j]==a[k][0])
			{

for(l=0;l < strlen(a[i])-j;l++)
{a1[l]=a[i][j+l];a2[l]=a[k][l];}
				a1[l]='\0';
				a2[l]='\0';
				if(!strcmp(a1,a2))
				{
					amg=1;
//printf("%s %s ",a1,a2);
					//printf("AMBIGUOUS!");
				}
			}

		}
	}
}
/*
if(!amg  || amg==1)
{*/
int found;

for(i=0;i<strlen(str);)
{
if(xxx)break;
found=0;
	for(j=0;j<n;j++)
	{
		if(str[i]==a[j][0])
		{
for(l=0;l<strlen(a[j]);l++)
{a1[l]=str[i+l];a2[l]=a[j][l];}
				a1[l]='\0';
				a2[l]='\0';
			if(!strcmp(a1,a2))
			{
					if(amg==0)printf("%s ",a2);
			found=1;
		i+=strlen(a2);
			}
		}
	}
	if(!found){xxx=1;/*printf("IMPOSSIBLE!");*/}
}
}//}
if(xxx==1 && amg==1)printf("IMPOSSIBLE!");
else if(xxx==1)printf("IMPOSSIBLE!");
else if(amg==1)printf("AMBIGUOUS!");
	return 0 ;
}