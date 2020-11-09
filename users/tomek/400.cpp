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
using namespace std;
#define max(a,b) a>b?a:b
#define min(a,b) a<b?a:b
#define vi vector<int>
#define vs vector<string>
#define pi pair<int,int>

int main()
{
 int g[8][8];
 int dir_x[8] = {-1, -1, 1, 1, 2, 2, -2, -2};
 int dir_y[8] = {-2, 2, -2, 2, -1, 1, -1, 1};
 
 int x1, y1, x2, y2;
 string s1, s2;
 cin >> s1 >> s2;
 x1 = s1[0] - 'a';
 y1 = s1[1] - '1';
 x2 = s2[0] - 'a';
 y2 = s2[1] - '1';
 
 if (x1 == x2 && y1 == y2) {
  cout << "To get from " << s1 << " to " << s2 << " takes 0 knight moves." << endl;
 return 0;
 }

 queue<pi> q;
 queue<int> steps;
 q.push(make_pair(x1,y1));
 steps.push(0);
 
 while (!q.empty()) {
  pi p = q.front();
  q.pop();
  int st = steps.front();
  steps.pop();
  
  for (int i=0; i<8; ++i) {
   int nx = p.first + dir_x[i];
   int ny = p.second + dir_y[i];
   int nst = st+1;
   
   if (nx >= 0 && nx < 8 && ny >=0 && ny < 8) {
    if (nx == x2 && ny == y2) {
     cout << "To get from " << s1 << " to " << s2 << " takes " << nst << " knight moves." << endl;
     return 0;
    }
    q.push(make_pair(nx,ny));
    steps.push(nst);
   }
  }
 }
 
 assert(0);
 return 0;
}