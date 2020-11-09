//This is just the default template, you can remove it if you want.

#include <stdio.h>
#include <string.h>


int main ()
{
	char msg[60];
	int nosw=0;
	int j;
	char sw[60][4];
	
	scanf("%s",msg);
	scanf("%d",&nosw);
	for(int i=0;i<nosw;i++)
	{
		scanf("%s",sw[i]);
		
	}

	for(int i=0; i < (int)strlen(msg) ; i++)
	{
		for(j=0;j<nosw;j++)
		{
		
			if(msg[i] == sw[j][0])
			{
		//		printf("\n -----------");
				msg[i] = sw[j][2];
 				i=i-1;
				break;
			}
		}
	}

	printf("%s",msg);
		
	return 0 ;
}