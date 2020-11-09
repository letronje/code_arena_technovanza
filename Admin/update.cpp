#include <stdlib.h>
#include <stdio.h>
int  main()
{
	system("clear");
	system("rm -f admin");
	system("qmake -o Makefile admin.pro");
	system("make");
	FILE* fp;
	if((fp=fopen("admin","r"))!= NULL)
	{
		system("./admin");
		fclose(fp);
	}
	return 0;
}
