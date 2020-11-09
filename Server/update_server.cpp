#include <stdio.h>
#include <stdlib.h>
int main ()
{
	system("rm -f server");
	system("clear");
	system("qmake -o Makefile server.pro");
	system("make");
	return 0;
}
