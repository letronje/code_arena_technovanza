#include <stdio.h>
#include <stdlib.h>
int main ()
{
	system("rm -f client");
	system("clear");
	system("qmake -o Makefile client.pro");
	system("make");
	if (fopen("client","r")!=NULL) system("./client");
	return 0;
}
