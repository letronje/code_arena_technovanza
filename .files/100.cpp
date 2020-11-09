#include <iostream>
#include <string>
#include <algorithm>
#include <strstream.h>

using namespace std ;

string Asc(string s)
{
	sort(s.begin(),s.end());
	return s;
}
string Desc(string s)
{
	sort(s.begin(),s.end());
	reverse(s.begin(),s.end());
	return s;
}

int ToInt(string s)
{
	strstream ss;
	ss<<s;
	int si;
	ss>>si;
	return si;	
}
int main ()
{
	string s;
	cin>>s;
	int i,ctr=0;
 	while( ( i = ToInt(Desc(s)) - ToInt(Asc(s)) ) != ToInt(s) )  
	{
		strstream ss;
		ss<<i;
		ss>>s;
		if(s.length() !=4)
			s=string(4-s.length(),'0')+s;
		ctr++;
	}
	cout << ctr ;
	return 0;
}