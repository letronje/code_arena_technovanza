

#include <stdio.h>
#include <string.h>

int main ()
{
	long int award[] = {0 , 1000 , 2500 , 5000 , 10000 , 25000 , 50000 , 75000 , 125000};
	char ans[10][60];
	long int init[] = { 0 ,0 , 0 , 0 , 0 , 0 , 0 , 0 };
	for(int i=0;i< 7;i++)
	{
		scanf("%s",ans[i]);
	}

	for(int i=0;i< 7;i++)
	{
		for(int k=1;k<9;k++)
		{
			int count =0;
			for(int j=0;j < (int)strlen(ans[i])  ;j++)
			{
				if(ans[i][j] == 'R')
				{
					count++;
				}
				else
				{
					count=0;
				}
				if(count == k)
				{
				
					
					init[k-1] += award[k];
					if(i==6)
					{
						init[k-1] += award[k];
					}
					count = 0;	
				}
			}
		}
	}

	for(int i=0;i< 8;i++)
		printf("%ld ",init[i]);
	return 0 ;
}