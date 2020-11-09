#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>
#include <qstring.h>
#include <qstringlist.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qdatetime.h>
#include <qdir.h> 
#include <vector>

#define MAX_QUEUE_LEN  30
#define PER				10
#define REGISTER 	 	1
#define SEND_PROBLIST	2
#define START			3
#define SEND_PROB		4
#define SUBMIT			5
#define SEND_SCORE		6
#define SEND_RULES		7
#define TIME_UP			8
#define CAN_SUB			9
#define SEND_RESULT	10
#define SEND_EGS		11
#define SEND_CODETIME   12

using namespace std ;

struct cmp
{
	bool operator () ( pair <QString,double> p1 , pair<QString,double> p2)
	{
		return ( p1.second > p2.second) ;
	}
};

void sigchld_handler(int s)
{
	while( wait(NULL) > 0);
}

QString path ;
int new_fd ;

void Log(QString);
void AppendFile(QString ,QString );
QString ReadFile ( QString ) ;
void WriteFile ( QString , QString ) ;
void GetPath();
int GetPort();
void LogServerStart();
QString GetTeam();
QString GetCollege();
QString GetCourse();
int CheckDuplicate(const QString&);
void SendProbList();
void SendTotalTime(QString& );
QString GetProbName();
QString GetCurrTimeStr();
void GetOpenType(int&);
void SendProblem(QString& );
void GetEXE(QString& , QString& );
void GetCode(QString& , QString& );
int GetCurrTime();
int GetStartTime(QString& ,QString& );
int GetAddTime(QString& ,QString& );
double GetTotalTime(QString& );;
bool CheckReSub(QString& ,QString& ,double& );
void AddToQ(QString& ,QString& ,double& ,bool& );
void SendTesterOP(QString& );
void SendResult(QString&);
void Quit(const QString& = "Error" );
void SendStr(const QString& );
QString RecvString();
void SendEGS(const QString& s);	
void CountProbs(vector<int>& );
QString GetSubScore(const QString& ,const QString & );

int main( )
{
	GetPath();
	int port = GetPort();
	int sockfd;  
	struct sockaddr_in server_addr, client_addr; 
	int sin_size;
	struct sigaction sa;
	int yes=1;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		Log("Error Creating Socket");
		exit(1);
	}
	if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) 
	{
		Log("Error Setting Socket Options");
		exit(1);
	}       
	server_addr.sin_family = AF_INET;       
	server_addr.sin_port = htons(port);     
	server_addr.sin_addr.s_addr = INADDR_ANY; 
	memset(&(server_addr.sin_zero), '\0', 8); 

	if (bind(sockfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr))==-1) 	
	{
		Log("Error Binding");
		exit(1);
	}
	LogServerStart();
	if (listen(sockfd, MAX_QUEUE_LEN) == -1) 
	{
		Log(strerror(errno));
		exit(1);
	}

	sa.sa_handler = sigchld_handler; 
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
    
	if (sigaction(SIGCHLD, &sa, NULL) == -1)
  	{
		Log("SigAction Error");
		exit(1);
	}
	while(1) 
	{
	    sin_size = sizeof(struct sockaddr_in);
	    if ((new_fd = accept(sockfd,(struct sockaddr *)&client_addr,(socklen_t *)&sin_size)) == -1) 
			continue;
	   if (!fork())
	    { 
			close(sockfd); 
			
			int command;
			if(recvfrom(new_fd,&command,sizeof(int),0,(struct sockaddr *)&client_addr,(socklen_t *)&sin_size)==-1)
				Quit("Error receiving command from client ");
		
			if(command==REGISTER)//ok
			{
				QString team = GetTeam();	
				cout << team << endl ;fflush(stdout);
				int msg = CheckDuplicate(team);
				if(send(new_fd,&msg,sizeof(int),0)==-1)
					Quit("Registration Error");
				if(msg)
				{	
					AppendFile(path+".files/.user_list",team+"\n");
					system("mkdir -p "+path+"users");
					system("mkdir "+path+"users/"+team);
					WriteFile(path+"users/"+team+"/ttime",ReadFile(path+".files/ttime"));
					WriteFile(path+"users/"+team+"/college",GetCollege());
					WriteFile(path+"users/"+team+"/course",GetCourse());
					Log ( "\""+team + "\" Registered Successfully ");
				}
			}
			
			if(command == SEND_PROBLIST)//ok
			{
				QString tname = GetTeam();
				SendProbList();
			}
			
			if(command == START ) //ok
			{
				QString tname = GetTeam();
				SendTotalTime(tname);
			}
			
			if(command == SEND_PROB)//ok
			{
				QString tname = GetTeam( );
				QString pname = GetProbName( );
				QString client_current_time ;
				bool refresh = false , first_time = true ;
				if(recv(new_fd,&refresh,sizeof(bool),0)==-1)
					Quit("Error Sending Problem to Team "+tname);
				if(refresh)
				{
					SendProblem(pname);
					Log("\""+pname + "\" => \"" + tname +"\" (Refresh)");
					goto Done;
				}	
				if(recv(new_fd,&first_time,sizeof(bool),0)==-1)
					Quit("Error Sending Problem "+pname+" to Team "+tname);
				client_current_time = RecvString() ;
				if(first_time)
				{
					WriteFile(path+"users/"+tname+"/"+pname+"st",client_current_time);
					WriteFile(path+"users/"+tname+"/"+pname+"at","0");
					Log ( "\""+tname + "\" Opened \"" + pname + "\"");
				}	
				else
					Log ( "\""+tname + "\" Re-opened \"" + pname + "\"");
				SendProblem(pname);
				Done:
				;
			}
			if(command == SUBMIT) // ok
			{
				QString tname = GetTeam();
				QString pname = GetProbName();
				GetEXE(tname,pname);
				GetCode(tname,pname);
				int st = GetStartTime(tname,pname);
				int ct;
				recv(new_fd,&ct,sizeof(int),0);
				int at = GetAddTime(tname,pname);
				int tpt = at + (ct-st);
				WriteFile(path+"users/"+tname+"/"+pname+"at",QString::number(tpt));
		  		double tt = GetTotalTime(tname);
				double score = pname.toDouble()*(0.3+(0.7*tt*tt)/((10.0*double(tpt)*double(tpt))+tt*tt)) ;
				bool resub = CheckReSub(tname,pname,score);
				AddToQ(tname,pname,score,resub);
			}
			
			if(command == SEND_SCORE)
			{
				QString tname = GetTeam();
			
				////////
				
				QString subs="<center><table border=1><tr><th align=center></th>";
				QString probs = ReadFile(path+".files/probs");
				int start = 0 ,i ,e;
				vector<int> problems;
				while ( ( i = probs.find("<g>",start) ) != -1 )
				{
					i +=QString("<g>").length() ;
					e = probs.find("</g>",i)-1;
					QString group = probs.mid(i,e-i+1).simplifyWhiteSpace();
					QStringList sl = QStringList::split(" ",group);
					for(QStringList::Iterator k=sl.begin();k!=sl.end();k++)
						problems.push_back((*k).toInt());
					start = i + 1 + QString("</g>").length();
				}
				sort(problems.begin(),problems.end());
				for(uint i=0;i<problems.size();i++)
					subs += "<td bgcolor=\"#feddff\" align=center><b><font color=\"darkMagenta\">"+QString::number(problems.at(i))+"</font></b></td>";
				subs += "</tr>";
				
				vector < pair <QString,double> > rows ;
				QFile f(path+".files/.user_list");
				if ( f.open( IO_ReadOnly) )
				{
					QTextStream s ( & f);
					while ( s.atEnd( ) == false )
					{
						pair <QString,double> row ;
						row.second=0.0;
						QString team=s.readLine().simplifyWhiteSpace();
						QString bgcolor;
						if(team==tname)
							bgcolor="#f8ff6f" ;
						else	
							bgcolor="#faf8eb";
						row.first += "<tr ><th bgcolor=\"#e3e1ff\" align=center><b><font color=\"darkBlue\">"+team+"</font></b></th>";
						for( uint i=0;i<problems.size();i++)
						{
							QString subscore = GetSubScore(team,QString::number(problems.at(i)));
							row.second += subscore.toDouble();							
							if(subscore == "X")
								subscore = "<font color=\"red\">X</font>";								
							else
								subscore = "<font color=\"darkGreen\">"+subscore+"</font>";
							row.first += "<td bgcolor=\""+bgcolor+"\" align=center>"+subscore+"</td>";
						}
						row.first +="</tr>";
						rows.push_back(row);
					}	
					f.close();
				}
				else
					Quit("Error Sending Score");
				sort(rows.begin(),rows.end(),cmp());
				for(uint i=0;i<rows.size();i++)
					subs += rows.at(i).first ;
				subs += "</table></center>" ;
				SendStr(subs);
				
				/////////
				//Log("Submissions => \""+tname+"\"");
			}
			
			if(command == SEND_RULES)
			{
				SendStr(ReadFile(path+".files/rules"));
				Log("Rules Sent");
			}
			if(command == TIME_UP)
			{
				QString tname=GetTeam();
				QString timeups = ReadFile(path+".files/.user_timeup");
				if(timeups.find(tname)==-1)
				{
					AppendFile(path+".files/.user_timeup",tname+"\n");
					Log("\""+tname + "\" => Time Up");
				}	
			}
			if(command == CAN_SUB)
			{
				QString tname=GetTeam();
				bool cansub=true;
				QString teams_timeup=ReadFile(path+".files/.user_timeup");
				if(teams_timeup.find(tname)!=-1)
					cansub=false;
				if(send(new_fd,&cansub,sizeof(bool),0)==-1)
					Quit("Error Sending Permission to submit to Team "+tname);
				if(!cansub )
				{
					QString l=ReadFile(path+".files/log.txt");
					if(l.find("\""+tname +"\" => X Submit X") == -1)
						Log("\""+tname +"\" => X Submit X");
				}	
			}
			
			if(command == SEND_RESULT)
			{
				QString tname=GetTeam( );
				bool result_available = true ;
				if((!QFile::exists(path+".files/result")) || (!QFile::exists(path+"users/"+tname+"/"+"result")))
				{
				    Log("unvailable");
				    result_available = false;
				}    
				if(send(new_fd,&result_available,sizeof(bool),0)==-1)
					Quit("Error Sending Result");
				if(result_available)
				{
					SendTesterOP(tname);
					SendResult(tname);
				}	
			}
			if(command == SEND_EGS)
			{
				QString tname=GetTeam( );
				QString pname = GetProbName();
				bool refresh = false ;
				if(recv(new_fd,&refresh,sizeof(bool),0)==-1)
					Quit("Error Sending Examples to Team "+tname+ " for "+pname);
				SendEGS(ReadFile(path+".files/."+pname+"e"));
				if(refresh)
					Log("\"" +tname +"\" => examples of \""+pname+"\" (refresh)");
				//else
				//	Log("\"" +tname +"\" => examples of \""+pname+"\"");
			}	
			if(command == SEND_CODETIME)
			{
				QString tname=GetTeam( );
				QString pname = GetProbName();
			
				QFile ftemp (path+"users/"+tname+"/"+pname+"st");
				int st = 0 ;
				if ( ftemp.open(IO_ReadOnly) )
				{
					int h,m,s;
					QTextStream stream ( &ftemp ) ;
					stream >> h;
					stream >> m;
					stream >> s ;
					st = h*3600 + m*60 + s ;
					ftemp.close();
				}
				if(send(new_fd,&st,sizeof(int),0)==-1)
					Quit("Error Sending Starting Time to \""+tname+ "\" for \""+pname+"\"");
			
				
				int at = ReadFile(path+"users/"+tname+"/"+pname+"at").toInt();
				if(send(new_fd,&at,sizeof(int),0)==-1)
					Quit("Error Sending Add Time to Team "+tname+ " for "+pname);
				
				QFile ft(path + ".files/ttime");
				int tt = 0 ;
				if ( ft.open(IO_ReadOnly) )
				{
					int h,m,s;
					QTextStream stream ( &ft ) ;
					stream >> h;
					stream >> m;
					stream >> s ;
					tt = h*3600 + m*60 + s ;
					ft.close();
				}
				if(send(new_fd,&tt,sizeof(int),0)==-1)
					Quit("Error Sending Total Time to Team "+tname+ " for "+pname);
			}
			close(new_fd);
			exit(0);
		}
		close(new_fd);  
	}	
	return 0;
}
void SendResult(QString& tname)
{
	QString res = ReadFile(path+".files/result");
	int resl = res.length();
	if(send(new_fd,&resl,sizeof(int),0)==-1)
		Quit("Error Sending Result");
	for( uint i=0;i<res.length();i++)
	{
		char c=(QChar)res[i];
		if(send(new_fd,&c,sizeof(char),0)==-1)
			Quit("Error Sending Result");
	}
	Log("Result => \""+tname+"\"");
}

void SendTesterOP(QString& tname)
{
	QString top = ReadFile(path+"users/"+tname+"/result");
	int topl = top.length();
	if(send(new_fd,&topl,sizeof(int),0)==-1)
		Quit("Error Sending Tester OP");
	for( uint i=0;i<top.length();i++)
	{
		char c=(QChar)top[i];
		if(send(new_fd,&c,sizeof(char),0)==-1)
			Quit("Error Sending Tester OP");
	}
}
void Log(QString s)
{
	QFile f(path+".files/log.txt");
	if(f.open(IO_WriteOnly | IO_Append))
	{
		QTextStream stream (&f);
		stream << s+"<br>";
		cout << s+"\n";fflush(stdout);
		f.close();
	}
	else
	{
		cout << "\nLog Error\n";
		exit(1);
	}
}
void AppendFile(QString fname,QString s)
{
	QFile f(fname);
	if(f.open(IO_WriteOnly | IO_Append))
	{
		QTextStream stream (&f);
		stream << s;
		f.close();
	}
	else
		Quit("Error Appending To " + fname);
}
QString ReadFile ( QString fname ) 
{
	QString data ;
	QFile f ( fname) ;
	
	if ( f.open( IO_ReadOnly) )
	{
		QTextStream s ( & f);
		while ( s.atEnd( ) == false )
		{
			 data += s.readLine() ;
			 if ( s.atEnd() == false )
			 	 data += "\n";
		}
		f.close();
	}
	else
		Quit("Error Reading File "+fname);
	return data ;
}
void WriteFile ( QString fname , QString data ) 
{
	QFile f ( fname) ;
	
	if ( f.open( IO_WriteOnly) )
	{
		QTextStream s ( & f);
		s << data;
		f.flush();
		f.close();
	}
	else
		Quit("Error Writing To File " + fname);
}
void GetPath()
{
	path = QDir::homeDirPath ()+"/";
}
int GetPort()
{
	QString p = ReadFile(path+".files/.port");
	return p.toInt();
}
void LogServerStart()
{
	AppendFile(path+".files/masterlog.txt","\n"+ReadFile(path+".files/log.txt"));
	WriteFile(path+".files/log.txt","");
	QTime ctime  = QTime::currentTime();
	QString msg = "Started ( " + QString::number(ctime.hour())+":" ;
	msg += QString::number(ctime.minute()) + ":" + QString::number(ctime.second())+" )";
	msg += " Port : " + QString::number(GetPort()) ;
	Log(msg);
}
QString GetStr()
{
	int len;
	if(recv(new_fd,&len,sizeof(int),0)==-1)
		Quit();
	QString str;
	for(int i=0;i<len;i++)
	{
		char c;
		if(recv(new_fd,&c,sizeof(char),0)==-1)
			Quit();
		str += c;
	}
	return str;
}
QString GetTeam()
{
	return GetStr();
}
QString GetCollege()
{
	return GetStr();
}
QString GetCourse()
{
	return GetStr();
}
QString GetProbName()
{
	return GetStr(); 
}

int CheckDuplicate(const QString& name)
{
	int msg =1;
	QFile f(path+".files/.user_list");
	if ( f.open( IO_ReadOnly) )
	{
		QTextStream s ( &f);
		while ( s.atEnd( ) == false )
		{
			if(s.readLine() == name)
			{
				msg=0;
				break ;
			}	
		}
		f.close();
	}
	return msg ;
}	
void SendProbList()
{
	SendStr(ReadFile(path+".files/probs"));
}
void SendTotalTime(QString& tname)
{
	QFile fd (path+"users/"+tname+"/ttime");
	if ( fd.open(IO_ReadOnly) )
	{
		int temp;
		QTextStream stream ( &fd ) ;
		stream >> temp;
		if(send(new_fd,&temp,sizeof(int),0)==-1)
			Quit("Error Sending Total Time");
		stream >> temp;
		if(send(new_fd,&temp,sizeof(int),0)==-1)
			Quit("Error Sending Total Time");
		stream >> temp;
		if(send(new_fd,&temp,sizeof(int),0)==-1)
			Quit("Error Sending Total Time");
		fd.close();
		//Log("Total Time => \""+tname+"\"");
	}
	else
		Quit ("Error Opening File For Sending Total Time To Team " + tname);
}	
/*QTime ct = QTime::currentTime();
	QString t=QString::number(ct.hour()) + " " + QString::number(ct.minute());
	t += " " + QString::number(ct.second())*/
void SendProblem(QString& pname)
{
	QString prob = ReadFile(path+".files/"+pname+".txt");
	int len = prob.length();
	if(send(new_fd,&len,sizeof(int),0)==-1)
		Quit("Error Sending Problem1 "+pname);
	for(int i=0;i<len;i++)
	{	
		char c=(QChar)prob[i];
		if(send(new_fd,&c,sizeof(char),0)==-1)
			Quit("Error Sending Problem2 "+pname);
	}	
}
void GetEXE(QString& tname,QString& pname)
{
	int exesize ;
	if(recv(new_fd,&exesize,sizeof(int),0)==-1)
		Quit("Error receiving EXE of problem "+pname +" of Team "+tname);
	QByteArray a(exesize);
	for(int i=0;i<exesize;i++)
	{
		int temp;
		if(recv(new_fd,&temp,sizeof(int),0)==-1)
			Quit("Error Receiving EXE Of Problem "+pname +" Of Team "+tname);
		a[i]=temp ;
	}
	QFile fout(path+"users/"+tname+"/"+pname);
	if(fout.open(IO_Raw|IO_WriteOnly))
	{
		fout.writeBlock(a);
		fout.close();
	}
	system("chmod +rwx " + path+"users/"+tname+"/"+pname);
}

void GetCode(QString& tname,QString& pname)
{
       WriteFile(path+"users/"+tname+"/"+pname+".cpp",RecvString());
}

int GetCurrTime()
{
	//QTime crt = QTime::currentTime();
	//return ( crt.hour()*3600 +  crt.minute()*60 + crt.second() );
    return RecvString().toInt();
}
int GetStartTime(QString& tname,QString& pname)
{
	QFile fd (path+"users/"+tname+"/"+pname+"st");
	int st = 0 ;
	if ( fd.open(IO_ReadOnly) )
	{
		int h,m,s;
		QTextStream stream ( &fd ) ;
		stream >> h;
		stream >> m;
		stream >> s ;
		st = h*3600 + m*60 + s ;
		fd.close();
	}
	else
		Quit("Error Reading Start Time");
	return st ;
}

int GetAddTime(QString& tname,QString& pname)
{
	QString at = ReadFile(path+"users/"+tname+"/"+pname+"at");
	return at.toInt();
}
double GetTotalTime(QString& tname)
{
	QFile ftemp (path + "users/" + tname + "/ttime");
	double tt = 0 ;
	if ( ftemp.open(IO_ReadOnly) )
	{
		int h,m,s;
		QTextStream stream ( &ftemp ) ;
		stream >> h;
		stream >> m;
		stream >> s ;
		tt = h*3600 + m*60 + s ;
		ftemp.close();
	}
	else
		Quit("Error Reading Total Time Of "+tname);
	return tt ;
}
bool CheckReSub(QString& tname,QString& pname,double& score)
{
		
	bool resub=false;
	QStringList sl = QStringList::split("\n",ReadFile(path+".files/queue"));
	for( QStringList::Iterator i=sl.begin();i!=sl.end();i++)
	{
		QString sub = (*i); 
		
		int j=sub.find(tname);
		if(j==-1)
			continue ;
		int k=sub.find(pname,j+tname.length());
		if(k==-1)
			continue ;
		score -= pname.toDouble()*PER/100.0;
		resub =true ;
	}
	if(score < 0.0)
		score=0.0;
	if(send(new_fd,&score,sizeof(double),0)==-1)
		Quit("Error Sending Score");
	return resub ;
}	
void AddToQ(QString& tname,QString& pname,double& score,bool& resub)
{	
	QString sub  = QString( "%1 %2 %3\n" ).arg(tname,-12).arg( pname,-4).arg(QString::number(score),-10);
	//AppendFile(path+".files/queue","\n"+tname+" "+pname+" "+QString::number(score));
	AppendFile(path+".files/queue",sub);
	QString logmsg ;
	if(resub)
		logmsg += " ReSubmitted ";
	else
		logmsg += " Submitted ";
	Log( "\""+tname + "\""+logmsg + "\""+pname + "\" For \"" + QString::number(score) +"\"");
}

void Quit(const QString& msg )
{
	Log("<font color=\"darkRed\">"+msg+"</font>");
	close(new_fd);
	exit(1);
}
void SendStr(const QString& s)
{
	int len = s.length();
	if(send(new_fd,&len,sizeof(int),0)==-1)
		Quit("Error sending string");
	for(int i=0;i<len;i++)
	{
		char c = QChar(s.at(i));
		if(send(new_fd,&c,sizeof(char),0)==-1)
			Quit("Error sending string");
	}	
}
QString RecvString()
{
	int len ;
	QString qs;
	if(recv(new_fd,&len,sizeof(int),0)==-1)
		Quit("Error receiving string");
	
	for(int i=0;i<len;i++)
	{
		char c ;	
		if(recv(new_fd,&c,sizeof(char),0)==-1)
			Quit("Error receiving string");
		qs+=c;
	}	
	return qs;
    
}
void SendEGS(const QString& s)
{
	SendStr(s);
}
void CountProbs(vector<int>& v)
{
	QString probs = ReadFile(path+".files/probs");
	int start = 0 ,i ,e;
	v.clear();
	while ( ( i = probs.find("<g>",start) ) != -1 )
	{
		i +=QString("<g>").length() ;
		e = probs.find("</g>",i)-1;
		QString group = probs.mid(i,e-i+1).simplifyWhiteSpace();
		QStringList sl = QStringList::split(" ",group);
		for(QStringList::Iterator k=sl.begin();k!=sl.end();k++)
			v.push_back((*k).toInt());
		start = i + 1 + QString("</g>").length();
	}
	sort(v.begin(),v.end());
}

QString GetSubScore(const QString& team,const QString & prob)
{
	QString subs = ReadFile(path+"/.files/queue").simplifyWhiteSpace();
	int j=subs.findRev(team+" "+prob);
	QString scr;
	if(j==-1)
		return "X";
	int k;
	j += QString(team+" "+prob).length()+1;
	k=j;
	j = subs.find(" ",j)-1;
	if(j==-2)
		j=subs.length()-1;
	scr = subs.mid(k,j-k+1);
	return  scr;
}
