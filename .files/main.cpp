#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <qstring.h>
#include <qfile.h>
#include <qdir.h>
#include <qtextstream.h>
#include <iostream>

using namespace std;

#define REG        			1
#define GETPROBLIST 	2
#define START      			3
#define GETPROB    		4
#define SUBMIT     		5
#define GETSCORE   		6
#define GETRULES 		7
#define TIMEUP	  		8
#define CANSUB	  		9
#define GETRESULT		10
#define GETEGS			11
#define GETCT			12

int sockfd;
QString path ;

void 		GetPath			();
QString 	GetServerIP		();
void 		SendEXE  		();
int		GetPort			();
QString		RecvStr			();
QString 		RecvTeam		();
void 		SendStr			(const QString&);
void 		SendCode		(const QString&);
void 		SendTeam		(const QString&);
void 		SendProb		(const QString&);
void 		SendCollege		(const QString&);
void 		SendCourse		(const QString&);
QString 		ReadFile 			(const QString&);
void 		RecvProb		(const QString&);
void		Quit			(const QString& msg = "");
void 		GetHost			(hostent*& ,const QString& );
void 		WriteFile 			( const QString& , const QString& ) ;
void 		AppendFile		(const QString& , const QString&);

int main(int argc,char *argv[])
{
	argc++;
	GetPath ();
	hostent *he ;
	GetHost(he,GetServerIP());
	int port = GetPort();
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
		Quit();
	struct sockaddr_in server_addr;              // connector's address information 
    	server_addr.sin_family = AF_INET;            // host byte order 
    	server_addr.sin_port   = htons(port);        // short, network byte order 
    	server_addr.sin_addr   = *((struct in_addr *)he->h_addr);
    	memset(&(server_addr.sin_zero),'\0',8);      // zero the rest of the struct 

	if (connect(sockfd, (struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1)
		Quit();
	
	int command = atoi(argv[1]);
	if(send(sockfd,&command,sizeof(int),0)==-1)
		Quit();
	if(command==REG)//ok
	{
		SendTeam(argv[2]);
		int mesg=1;
		if(recv(sockfd,&mesg,sizeof(int),0)==-1)
			Quit();
		WriteFile(path+".reg",QString::number(mesg));
		if(mesg)
		{	
			SendCollege(argv[3]);
			SendCourse(argv[4]);
		}
	}
	
	if(command==GETPROBLIST) //ok
	{
		SendTeam(argv[2]);
		WriteFile( path+ ".probs",RecvStr());
	}
	
	if(command==START)//ok
	{
		SendTeam(argv[2]);
		int ti;
		QString ttime="" ;
		for(int i=1;i<=3;i++)
		{
			if(recv(sockfd,&ti,sizeof(int),0)==-1)
				Quit();
			ttime += QString::number(ti)+" ";
		}
		WriteFile(path + ".ttime",ttime);
	}
	
	if(command==GETPROB)//ok
	{
		SendTeam(argv[2]);//send team
		QString prob=argv[3];
		SendProb(prob);//send prob
		bool refresh = bool(atoi(argv[4]));
		if(send(sockfd,&refresh,sizeof(bool),0)==-1)
			Quit();
		if(refresh)
			RecvProb(prob);
		else
		{
			bool first_time= bool(atoi(argv[5]));
			if(send(sockfd,&first_time,sizeof(bool),0)==-1)
				Quit();
			QTime ct = QTime::currentTime();
			QString t=QString::number(ct.hour()) + " " + QString::number(ct.minute());
			t += " " + QString::number(ct.second()) ;
			SendStr(t);
			RecvProb(prob);	
		}
	}
	if(command==SUBMIT) // ok
	{
		SendTeam(argv[2]);
		SendProb(argv[3]);
		SendEXE();
		SendCode(ReadFile(QDir::homeDirPath()+"/arena/"+argv[3]+".cpp"));
		QTime ct=QTime::currentTime();
		int crt=ct.hour()*3600+ct.minute()*60+ct.second();
		send(sockfd,&crt,sizeof(int),0);
		double score;
		if(recv(sockfd,&score,sizeof(double),0)==-1)
			Quit();
		WriteFile(path+".score",QString::number(score));
	}
	
	if ( command == GETSCORE )// Score
	{
		SendTeam(ReadFile(path+".team"));
		/*
		int no_teams;
		if(recv(sockfd,&no_teams,sizeof(int),0)==-1)
			Quit();
		int no_probs;
		if(recv(sockfd,&no_probs,sizeof(int),0)==-1)
			Quit();
		QString allscores;
		for(int i=1;i<=no_teams;i++)
		{		
			QString team=RecvTeam();
			int pname;
			double score;
			allscores +="Team : " + team + "\n\n";
			allscores +=  QString("%1\t\t%2\n\n").arg("Problem",-7).arg("Submitted For",-15);
			for(int j=1;j<=no_probs;j++)
			{
				if(recv(sockfd,&pname,sizeof(int),0)==-1) 
					Quit();
				if(recv(sockfd,&score,sizeof(double),0)==-1)
					Quit();
				if(score >= 0)
					allscores += QString("%1\t\t%2\n").arg(pname,-7).arg(score,-15);
				else
					allscores += QString("%1\t\t%2\n").arg(pname,-7).arg("<Not Submitted>",-15);
			}
			allscores += "\n********************************************\n" ;
		}
		*/
		WriteFile(path+".allscores",RecvStr());
	}
	if(command == CANSUB )
	{
		SendTeam(argv[2]);
		bool cansub=true;
		if(recv(sockfd,&cansub,sizeof(int),0)==-1)
			Quit();
		WriteFile(path+".sub",QString::number(cansub));
	}
	
	if(command == GETRULES)
	{
		WriteFile(path+".rules",RecvStr());
	}
	
	if(command == TIMEUP)
	{
		string team = argv[2];
		int tlen = team.length();
		if(send(sockfd,&tlen,sizeof(int),0)==-1)
			Quit();
		if(send(sockfd,team.c_str(),sizeof(char)*team.length(),0)==-1)
			Quit();
	}
	
	if(command == GETRESULT)
	{
		SendTeam(argv[2]);
		bool result_available=true;
		if(recv(sockfd,&result_available,sizeof(bool),0)==-1)
			Quit();
		if(result_available)
		{	
			WriteFile(path+".testerop",RecvStr());
			WriteFile(path+".result",RecvStr());
		}
		else
		{
			WriteFile(path+".testerop","Testing Has Not Been Started Yet,Please Try After Some Time");
			WriteFile(path+".result","Result Currently UnAvailable");
		}	
	}
	if(command == GETEGS)
	{
		QString tname = ReadFile(path+".team");
		QString pname = ReadFile(path+".p");
		SendTeam(tname);
		SendProb(pname);
		bool refresh = bool(atoi(argv[2]));
		if(send(sockfd,&refresh,sizeof(bool),0)==-1)
			Quit();
		WriteFile(	path+".egs",RecvStr());
	}
	if(command == GETCT)
	{
		SendTeam(ReadFile(path+".team"));
		SendProb(ReadFile(path+".p"));
		int st,at,tt ;
		if(recv(sockfd,&st,sizeof(int),0)==-1)
			Quit();
		if(recv(sockfd,&at,sizeof(int),0)==-1)
			Quit();
		if(recv(sockfd,&tt,sizeof(int),0)==-1)
			Quit();
		WriteFile(path+"st",QString::number(st));
		WriteFile(path+"at",QString::number(at));
		WriteFile(path+"tt",QString::number(tt));
		
	}
	close(sockfd);
	return 0;
}
QString ReadFile ( const QString& fname ) 
{
	QString data = "-1";
	QFile f ( fname) ;
	
	if ( f.open( IO_ReadOnly) )
	{
		data="";
		QTextStream s ( & f);
		while ( s.atEnd( ) == false )
		{
			 data += s.readLine() ;
			 if ( s.atEnd() == false )
			 	 data += "\n";
		}
		f.close();
	}
	return data ;
}
void GetPath()
{
	path = QDir::homeDirPath()+"/.files/";
}

QString GetServerIP()
{
	return ReadFile(path+".serverip"); 
}

void GetHost(hostent*& h,const QString& sip)
{
	if ( ( h = gethostbyname(sip) ) == NULL)
    	{
     		perror("gethostbyname");
         	exit(1);
    	}
}

int GetPort()
{
	return ReadFile(path+".port").toInt();
}
void SendCode(const QString& s)
{
    SendStr(s);
}
void SendStr( const QString& s)
{
	int len=s.length();
	if(send(sockfd,&len,sizeof(int),0)==-1)
		Quit();
	
	for(int i=0;i<len;i++)
	{	
		char c=QChar(s.at(i));	
		if(send(sockfd,&c,sizeof(char),0)==-1)
			Quit();
	}	
}
void SendTeam( const QString& t)
{
	SendStr(t);
}
void SendProb( const QString& p)
{
	SendStr(p);
}
void SendCollege( const QString& p)
{
	SendStr(p);
}
void SendCourse( const QString& p)
{
	SendStr(p);
}
void WriteFile ( const QString& fname , const QString& data ) 
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
		Quit();
}
QString RecvStr()
{	
	int len;
	if(recv(sockfd,&len,sizeof(int),0)==-1)
		Quit();
	QString str;
	for(int i=0;i<len;i++)
	{
		char c;
		if(recv(sockfd,&c,sizeof(char),0)==-1)
			Quit();
		str += c;
	}
	return str;
}
QString RecvTeam()
{	
	return RecvStr();
}	
void AppendFile(const QString& fname, const QString& s)
{
	QFile f(fname);
	if(f.open(IO_WriteOnly | IO_Append))
	{
		QTextStream stream (&f);
		stream << s;
		f.close();
	}
	else
		Quit();
}		
void Quit(const QString& msg )
{
	perror(msg);
	exit(1);
}
void RecvProb(const QString& pname)
{
	QString prob;
	int flen=1;
	if(recv(sockfd,&flen,sizeof(int),0)==-1)
		Quit();
	for(int i=0;i<flen;i++)
	{
		char c;
		if(recv(sockfd,&c,sizeof(char),0)==-1)
			Quit();
		prob +=c;
	}
	WriteFile(path+pname+"p",prob);
}
void SendEXE()
{
	int file_size = QFileInfo(path+"test").size ();
	if ( send(sockfd,&file_size,sizeof(int),0) == -1)
		Quit();
	/*fstream f;
	f.open(path+"test",ios::in|ios::binary);
	for ( ulong ctr = 0 ; ctr < file_size ; ctr ++ ) 
	{
		char ch ;
		f.get(ch);
		int i=ch;
		if(send(sockfd,&i,sizeof(int),0)==-1)
			Quit();
	}
	f.close();
	*/
	QFile fin(path+"test");
	if(fin.open( IO_Raw | IO_ReadOnly))
	{
		QByteArray a = fin.readAll();
		for(int i=0;i<a.size();i++)
		{
			int temp=a[i];
			if(send(sockfd,&temp,sizeof(int),0)==-1)
				Quit();
		}
		fin.close();
	}
}		
