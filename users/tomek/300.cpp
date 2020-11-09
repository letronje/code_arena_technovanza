#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iterator>
#include <sstream>
using namespace std;
#define max(a,b) a>b?a:b
#define min(a,b) a<b?a:b
#define vi vector<int>
#define vs vector<string>
#define pi pair<int,int>
int score[8];

int main() {
 int array[9] = { 0, 1000, 2500, 5000, 10000, 25000, 50000, 75000, 125000 };
 
 for (int i=0; i<7; ++i) {
  string s;
  cin >> s;
  
  for (int j=1; j<=8; ++j) {
   string ser;
   for (int k=0; k<j; ++k)
    ser += 'R';
    
   int k=0, count=0;
   while (k < s.size()) {
    int p = s.find(ser, k);
    if (p < s.size()) {
     ++count;
     k = p+ser.size();
    } else
     break;
   }
   
   if (i != 6)
    score[j-1] += count * array[j];
   else
    score[j-1] += count * array[j] * 2;
  }
 }
 
 cout << score[0];
 for (int i=1; i<8; ++i)
  cout << ' ' << score[i];
 cout << endl;
 return 0;
}