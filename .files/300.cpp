#include <iostream> 
 
#include <string> 
using namespace std; 
 
int main() 
{ 
    int award[9]={0,1000,2500,5000,10000,25000,50000,75000,125000}; 
    string s[7]; 
    for(int i=0;i<7;i++) 
    { 
        string temp; 
        cin>>temp; 
        s[i]=temp; 
    } 
    int ans[9]={0,0,0,0,0,0,0,0,0}; 
    //first 6 rnds 
    for(int i=0;i<6;i++) 
    { 
        string curr = s[i]; 
 
        string r=""; 
        for(int k=1;k<=8;k++) 
        { 
                r+='R'; 
                int j=-k; 
 
                while((j=(int)curr.find(r,j+k))!=-1) 
                {                 
                     ans[k]+=award[k]; 
                } 
        } 
    } 
    //for 7th rnd 
    string curr = s[6]; 
    string r=""; 
    for(int k=1;k<=8;k++) 
    { 
          r+='R'; 
          int j=-k; 
          while((j=curr.find(r,j+k))!=-1) 
          {                 
               ans[k]+=2* award[k]; 
          } 
    } 
 
    for(int i=1;i<9;i++) 
    { 
        cout<<ans[i]<<" "; 
    } 
 
    return 0; 
}