#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <queue>
#include <cassert>
#include <stack>
using namespace std;
#define max(a,b) a>b?a:b
#define min(a,b) a<b?a:b
#define vi vector<int>
#define vs vector<string>
#define pi pair<int,int>

vi array[50];
vs dict;
string orig;
int dp[50][50];

int main()
{
 int n;
 cin >> n;
 dict = vs(n);
	string ans = "";
	for (int i=0; i<n; ++i)
		cin >> dict[i];
  
 cin >> orig;
 for (int i=0; i<dict.size(); ++i) {
  int k=0;
  while (k < orig.size()) {
   int p = orig.find(dict[i], k);
   if (p >= orig.size())
    break;
   array[p].push_back(i);
   k = p+1;
   //cout << p << ", " << dict[i] << endl;
  }
 }
 
 stack<pi> s;
 for (int i=0; i<array[0].size(); ++i) { 
  if (dict[array[0][i]].size() < orig.size())
   s.push(make_pair(0, array[0][i]));
  else if (dict[array[0][i]] == orig) {
   ans = dict[array[0][i]];
   cout << ans << endl;
   return 0;
  }
 }
 
 while (!s.empty()) {
  pi p = s.top();
  s.pop();
  int pos = p.first + dict[p.second].size();
  //cout << pos << "; " << dict[p.second] << endl;
  
  for (int i=0; i<array[pos].size(); ++i) {
   int l = pos+dict[array[pos][i]].size();
   if (l == orig.size()) {
    if (ans != "") {
     cout << "AMBIGUOUS!" << endl;
     return 0;
    }
   
    int p1 = p.first;
    int prev = p.second;
    ans = dict[array[pos][i]];
    //cout << ans << endl;
    while (p1 >= 0) {
     //cout << p1 << ", " << dict[prev] << endl;
     ans = dict[prev] + " " + ans;
     if (p1 != 0) {
      prev = dp[prev][p1];
      p1 -= dict[prev].size();
     } else
      break;
    }
   } else if (l < orig.size()) {
    s.push(make_pair(pos, array[pos][i]));
    dp[array[pos][i]][pos] = p.second;
   }
  }
 }
 
 if (ans == "")
  cout << "IMPOSSIBLE!" << endl;
 else 
  cout << ans << endl;
 return 0;
}