//This is just the default template, you can remove it if you want.

#include <iostream>

using namespace std ;

int main ()
{
	int n;
	cin >> n;

	int *length = new int[n];
	int *count = new int[n];

	int i, k, l;
	char **dict = (char **) calloc(n, sizeof(char *));
	for (i=0; i<n; i++)
	{
		count[i] = 0;
		dict[i] = new char[50];
		cin >> dict[i];
		length[i] = strlen(dict[i]);
	}

	char *input = new char[50];
	cin >> input;

	char *output = new char[100];
	char* check = new char[50*n];
 
	int flag = 0;
	int len = strlen(input);
	for (k=0; k<len; k++)
	{
		for(l=0; l<n; l++)
		{
			if (dict[l][count[l]] == input[k])
			{
				count[l]++;
				if (count[l] == (length[l]-1))
				{
					count[l] = 0;
					strcat(output, dict[l]);
					strcat(check, dict[l]);
					
					if (k == len-1 )
						flag++;
					else
						strcat(output, " ");
				}
			}
		}
	}



	if (strcmp(check, input) == 0)
	{
		cout << output;
	}
	else 
	if (flag > 1)
			{
				cout << "AMBIGUOUS!";

		}
			else
			{
				cout << "IMPOSSIBLE!";
	
			}
	
	return 0 ;
}