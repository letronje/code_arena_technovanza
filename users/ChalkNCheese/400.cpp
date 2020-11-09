//This is just the default template, you can remove it if you want.

#include <iostream>

using namespace std ;

int main ()
{
 struct pt
 { 
  int x,y;
  int d;
 }q[100000];

 int b=0,f=0;
 
 pt tgt,src;
 char a,c;

 cin>>a>>src.y;
 cin>>c>>tgt.y;

 src.d=0;
 tgt.d=0;

 switch(a)
 {
  case 'a': src.x=1; break;
  case 'b': src.x=2; break;
  case 'c': src.x=3; break;
  case 'd': src.x=4; break;
  case 'e': src.x=5; break;
  case 'f': src.x=6; break;
  case 'g': src.x=7; break;
  case 'h': src.x=8; break;
 }
 switch(c)
 {
  case 'a': tgt.x=1; break;
  case 'b': tgt.x=2; break;
  case 'c': tgt.x=3; break;
  case 'd': tgt.x=4; break;
  case 'e': tgt.x=5; break;
  case 'f': tgt.x=6; break;
  case 'g': tgt.x=7; break;
  case 'h': tgt.x=8; break;
 }
 q[b]=src; f++;
 pt now,next;
 int flag=0;
 
 while(b!=f && flag==0)
 {
  now=q[b];
    
  next.d=now.d+1;
  next.x=now.x+1;
  next.y=now.y+2;
  if (next.x<9 && next.y<9 && next.x>0 && next.y>0)         
   {q[f]=next; f++; if (next.x==tgt.x && next.y==tgt.y) {flag=1;tgt=next;}}
  
  next.d=now.d+1;
   next.x=now.x-1;
   next.y=now.y+2;
   if (next.x<9 && next.y<9 && next.x>0 && next.y>0)         
    {q[f]=next; f++; if (next.x==tgt.x && next.y==tgt.y) {flag=1;tgt=next;}}
 
 next.d=now.d+1;
  next.x=now.x+1;
  next.y=now.y-2;
  if (next.x<9 && next.y<9 && next.x>0 && next.y>0)         
   {q[f]=next; f++; if (next.x==tgt.x && next.y==tgt.y) {flag=1;tgt=next;}}
 
 next.d=now.d+1;
  next.x=now.x-1;
  next.y=now.y-2;
  if (next.x<9 && next.y<9 && next.x>0 && next.y>0)         
   {q[f]=next; f++; if (next.x==tgt.x && next.y==tgt.y) {flag=1;tgt=next;}}

 next.d=now.d+1;
  next.x=now.x+2;
  next.y=now.y+1;
  if (next.x<9 && next.y<9 && next.x>0 && next.y>0)         
   {q[f]=next; f++; if (next.x==tgt.x && next.y==tgt.y) {flag=1;tgt=next;}}

  next.d=now.d+1;
  next.x=now.x+2;
  next.y=now.y-1;
  if (next.x<9 && next.y<9 && next.x>0 && next.y>0)         
   {q[f]=next; f++; if (next.x==tgt.x  && next.y==tgt.y) {flag=1; tgt=next;}}

  next.d=now.d+1;
  next.x=now.x-2;
  next.y=now.y+1;
  if (next.x<9 && next.y<9 && next.x>0 && next.y>0)         
   {q[f]=next; f++; if (next.x==tgt.x  && next.y==tgt.y) {flag=1; tgt=next;}}

  next.d=now.d+1;
  next.x=now.x-2;
  next.y=now.y-1;
  if (next.x<9 && next.y<9 && next.x>0 && next.y>0)         
   {q[f]=next; f++; if (next.x==tgt.x && next.y==tgt.y) {flag=1; tgt=next;}}

 b++;
 

 }
cout<<"To get from "<<a<<src.y<<" to "<<c<<tgt.y<<" takes "<<tgt.d<<" knight moves.";
 
}