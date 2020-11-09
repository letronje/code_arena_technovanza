//This is just the default template, you can remove it if you want.

#include <iostream>

using namespace std ;

int main ()
{
	char msg[50];
	cin >> msg;

	int n;
	cin >> n;

	char a[26];
	char x = 'A';
	for (int i=0; i<26; i++)
	{
		a[i] = x;
		x++;
	}

	int n1, n2, temp;
	char init;
	char fin;
	char st[3];
	for (int i=0; i<n; i++)
	{
		cin >> st;
		init = st[0];
		fin = st[2];
		n1 = init - 65;
		n2 = fin - 65;
		temp = a[n1];
		a[n1] = a[n2];
		a[n2] = temp;
	}

	int k;
	char y;
	int len = strlen(msg);
	char *orig = new char[len];
	for (int j=0; j<len; j++)
	{
		y = msg[j];
		for (k=0; k<26 && a[k] != y; k++);
		orig[j] = (char) (k+65);
	}
			
	cout << orig;
	return 0 ;
}