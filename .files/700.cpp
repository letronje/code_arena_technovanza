#include <string>
#include <iostream>

using namespace std;

int main()
{
	string key,code;

	cin>>key>>code;

	int ans=0;
	for(int i=0;i<code.length();i++)
	{
		int j = key.find(code.at(i),0)+1;
		if(j==0) 
			continue;
		if(j==10)
			 j=0;
		ans = ans*10 + j;	
	}
	cout<<ans;
}