//This is just the default template, you can remove it if you want.

#include <iostream>
#include <string.h>
#include <ctype.h>

using namespace std ;

int main ()
{
long int award[50] = { 0, 1000, 2500, 5000, 10000, 25000, 50000, 75000, 125000 } ;
long int awd7[50];
static long  int addmon[10];
char a[7][100];
int i,j,k,l,corr;
for(i=0;i<7;i++)
scanf("%s",a[i]);
int cont,as[7];

for(i=1;i<=8;i++)
{
awd7[i]=award[i]*2;
}

for(i=1;i<=8;i++)
{
int rrr;
for(j=0;j<7;j++)
{

as[j]=0;
	for(k=0;k<strlen(a[j]);)
	{	
rrr=k;
corr=0;
		cont=0;
		for(l=0;l<i;l++)
		{k++;

	if( (l+rrr) > strlen(a[j]) )
	{corr=0;cont=1; break;
//	addmon[i]+=award[i]*i; as[j]+=award[i]*i;
	}

	if(a[j][rrr+l]=='W')
   {corr=0;cont=1; break; }
corr++;
//if(cont) break;
				
		}

if(corr==i){
if(j!=6)
{addmon[i]+=award[i]*i; as[j]+=award[i]*i;}
else
{addmon[i]+=awd7[i]*i; as[j]+=awd7[i]*i;}
}

}

	}
//printf("%d ",as[j]);

//printf("%d ",as[j]);
	//if(cont){ continue;}

		
//printf(".. %d ",award[i]*i);
}

for(i=1;i<=8;i++)
printf("%ld ",addmon[i]);
	return 0 ;
}