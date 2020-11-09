//This is just the default template, you can remove it if you want.

#include <iostream>
#include <string>

using namespace std ;

int main ()
{
 int n,flag=0,i;
 char dic[51][51],word[51];
 cin>>n;
 for (i=0; i<n; i++) cin>>dic[i];
 cin>>word;
 
 for (i=0; i<n; i++) if (strcmp(word,dic[i])==0) {flag=1; cout<<word;}
 
 if (flag==0) cout<<"IMPOSSIBLE!";
}