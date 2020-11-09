#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <numeric>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <queue>
using namespace std;
typedef vector<int> VI;  typedef vector<vector<int> > VVI;
typedef vector<string> VS;  typedef vector<vector<string> > VVS;
typedef signed long long i64;  typedef unsigned long long u64;

int main()
{

	vector <string> a;
	string b;
	int size;
	cin>>size;
	for(int i=0;i<size;i++)
	{
		string temp;
		cin>>temp;
		a.push_back(temp);
	}
	cin>>b;
	int i, j, k, x, y, z, n;

  	VI prev(b.size()+1, -1);
  	prev[0] = -2;
  	for( i = 1; i <= b.size(); i++ )
  		for( j = 0; j < i; j++ ) if( prev[j] != -1 ) {
    if( find(a.begin(), a.end(), b.substr(j, i-j)) != a.end() ) {
      if( prev[i] != -1 || prev[j] == -3 ) prev[i] = -3;
      else prev[i] = j;
    }
  }
  if( prev[b.size()] == -1 ) {cout<<"IMPOSSIBLE!";return 0;}
  if( prev[b.size()] == -3 ) {cout<<"AMBIGUOUS!";return 0;}
  for( x = b.size(); prev[x]; x = prev[x] ) {
    b = b.substr(0, prev[x]) + " " + b.substr(prev[x]);
  }
  cout<<b;
}