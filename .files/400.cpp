#include <iostream>
#include <string>

#include <queue>
#include <vector>
using namespace std;

vector<vector<int> >grid (8,8);  
    
int func(queue<int> qr,queue<int> qc,queue<int> qcnt,int er,int ec)
{

    int row = qr.front();
    int col = qc.front();
    
    qr.pop();
    qc.pop();

    int cnt = qcnt.front();
    qcnt.pop();
    
    
    if(row==er && col==ec) return cnt;
    
    if( row+2<=7)
    {
        if(col+1<=7 && !grid.at(row+2).at(col+1))
        {
                grid.at(row+2).at(col+1)=1;
                qr.push(row+2);
                qc.push(col+1);
                qcnt.push(cnt+1);

        }
        if(col-1>=0 && !grid.at(row+2).at(col-1) )
        {
                grid.at(row+2).at(col-1)=1;        
                qr.push(row+2);
                qc.push(col-1);
                qcnt.push(cnt+1);

         }
    }
    if(row+1<=7 )
    {
    
        if( col-2>=0 && !grid.at(row+1).at(col-2) )
        {
                grid.at(row+1).at(col-2)=1;
                qr.push(row+1);
                qc.push(col-2);
                qcnt.push(cnt+1);

         }
        if(col+2<=7 && !grid.at(row+1).at(col+2))
        {
                grid.at(row+1).at(col+2)=1;
                qr.push(row+1);
                qc.push(col+2);
                qcnt.push(cnt+1);

         }
    }
    if(row-1>=0 )
    {

        if(col-2>=0 && !grid.at(row-1).at(col-2))
        {
                grid.at(row-1).at(col-2)=1;        
                qr.push(row-1);
                qc.push(col-2);
                qcnt.push(cnt+1);

         }
        if( row-1>=0 && col+2<=7 && !grid.at(row-1).at(col+2))
        {
                grid.at(row-1).at(col+2)=1;
                qr.push(row-1);
                qc.push(col+2);
                qcnt.push(cnt+1);
        }
    }
    if( row-2>=0 )
    {
        if(col-1>=0 && !grid.at(row-2).at(col-1))
        {
                grid.at(row-2).at(col-1)=1;        
                qr.push(row-2);
                qc.push(col-1);
                qcnt.push(cnt+1);
        }
        if( col+1<=7 && !grid.at(row-2).at(col+1))
        {
                grid.at(row-2).at(col+1)=1;        
                qr.push(row-2);
                qc.push(col+1);
                qcnt.push(cnt+1);
        }
    }
    
    return func(qr,qc,qcnt,er,ec);
}

int main()
{
    
    char t[6];

    while(gets(t)!=NULL)
    {
      for(int i=0;i<=7;i++)
      {
            for(int j=0;j<=7;j++)
            {
                        grid.at(i).at(j)=0;
            }
      }

    string s=t;

    int sr,sc,er,ec;
    sc = s[0]-'a';
    sr = s[1]-'1';
    
    ec = s[3]-'a';
    er = s[4]-'1';

    queue<int> qr;
    queue<int> qc;
    queue<int> qcnt;
    grid.at(sr).at(sc)=1;
    qr.push(sr);
    qc.push(sc);
    qcnt.push(0);
    
    cout<<"To get from "<<s[0]<<s[1]<<" to "<<s[3]<<s[4]<<" takes "<<func(qr,qc,qcnt,er,ec)<<" knight moves.\n";
    }
    return 0;
}