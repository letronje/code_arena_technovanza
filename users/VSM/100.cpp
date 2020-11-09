//This is just the default template, you can remove it if you want.

#include <iostream>
#include <math.h>
using namespace std ;

int solve(int);
void sort (int []);

int main ()
{
	int n;
	cin >> n;

	int no =solve(n);
  	cout << no;
	return 0;
}

int solve (int x)
{
	static int count = 0;
	count++;
	int temp = x;
	int a[4];
	int dig=0;
 	for (int i=0; i<4; i++)
	{
		dig = (temp/((pow(10,(double)(i)))));
		a[i] = dig%10;
	}
	sort(a);

	int large = 0;
	int small = 0;
	for (int i=0; i<4; i++)
	{
		large+= a[i]*(pow(10,(double)(i)));
		small+= a[i]*(pow(10,(double)(3-i)));
	}
	int diff = large - small;
	if (diff == x)
	{
		return ((count-1));
	}
	else
	{
		return (solve(diff));
	}

}

void sort(int a[])
{
	int temp;
	for (int i=3; i>0; i--)
	{
		for (int j=0; j<i; j++)
		{
			if (a[j] > a[j+1])
			{
				temp = a[j];
				a[j] = a[j+1];
				a[j+1] = temp;
			}
		}
	}
}