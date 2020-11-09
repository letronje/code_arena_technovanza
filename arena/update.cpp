#include <stdio.h>
#include <stdlib.h>
int main ()
{
	system("rm -f arena");
	system("clear");
	system("qmake -o Makefile arena.pro");
	system("make");
	if (fopen("arena","r")!=NULL) system("./arena");
	return 0;
}
