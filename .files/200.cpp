#include <string>
#include <iostream>
#include <numeric>

using namespace std ;

int main ()
{
	string visible ;
	cin>>visible;
	
	string orig,switched ;
	for( int i=0;i<26;i++)
		orig.push_back(65+i);
	switched=orig;
	
	int no_switches ;
	cin>>no_switches ;
		
	for( int i=0;i<no_switches;i++)
	{
		string s;
		cin>>s;
		char c1,c2;
		sscanf(s.c_str(),"%c:%c",&c1,&c2);
		int c1_index = switched.find(c1) ;
		int c2_index = switched.find(c2);
		char temp=switched.at(c1_index);
		switched.at(c1_index)=switched.at(c2_index);
		switched.at(c2_index)=temp ;
	}
	for( int i=0;i<visible.length();i++)
		cout << switched.at(visible.at(i)-65);
	return 0;
}