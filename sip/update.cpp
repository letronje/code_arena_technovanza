#include <stdio.h>
#include <stdlib.h>
int main ()
{
	system("rm -f sip");
	system("clear");
	system("qmake -o Makefile sip.pro");
	system("make");
	if (fopen("sip","r")!=NULL) system("./sip");
	return 0;
}
