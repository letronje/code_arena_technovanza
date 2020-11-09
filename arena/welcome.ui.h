#include <qinputdialog.h>
#include <qprocess.h>
#include <qmessagebox.h>
#include <unistd.h>
#include <qfile.h>
#include <qtextstream.h>
#include <sys/types.h>
#include <qevent.h>
#include <pwd.h>
#include <stdio.h>
#include <qdir.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <qstringlist.h>

#include "rules.h"
#include "options.h"
#include "ide.h"
#include "reg.h"
#include "decl.h"

using namespace std;

bool cansub = true ;
QString path_welcome;
ulong s_time ;//when contest started
ulong e_time ;
QProcess* pp = NULL ;
vector < vector <int> > problems;


//calculate path_welcome were hidden files are saved
bool welcome :: IsRegistered()
{
	QString t = ReadFile(path_welcome+".areg") ;
	if(t=="1")
		return true;
	else
		return false;
}

bool welcome :: GetTotalTime()
{	
	QString ttime = ReadFile(path_welcome+".ttime");
	QFile(path_welcome + ".ttime").remove();
	QString command = path_welcome + "./client ";
	command += "3 " + ReadFile(path_welcome+".team");
	system(command);
	if(!QFile::exists(path_welcome+".ttime"))
	{
		WriteFile(path_welcome+".ttime",ttime);
		return false;
	}	
	else
		return true;
}		
bool welcome :: LoadTeamName()
{
	QString tname = ReadFile(path_welcome+".team") ;
	if(tname == "-1")
		return false ;
	team->setText("Team : " + tname);
	return true ;
}
void welcome::LoadGroup()
{
	File->clear();
	int gindex = grp->currentText().toInt()-1;
	for(uint i=0;i<problems.at(gindex).size();i++)
		File->insertItem(QString::number(problems.at(gindex).at(i)));
}
bool welcome :: LoadProbs()
{
	grp->clear();
	File->setEnabled(true);
	Start->setText("<< GO >>");
	File->clear();
	QString probs = ReadFile(path_welcome+".probs");
	if(probs=="-1")
		return false;
	int start = 0 ,i ,e;
	problems.clear();
	while ( ( i = probs.find("<g>",start) ) != -1 )
	{
		i +=QString("<g>").length() ;
		e = probs.find("</g>",i)-1;
		if(e==-2)
			return false;
		QString group = probs.mid(i,e-i+1).simplifyWhiteSpace();
		QStringList sl = QStringList::split(" ",group);
		vector<int> v;
		for(QStringList::Iterator k=sl.begin();k!=sl.end();k++)
			v.push_back((*k).toInt());
		problems.push_back(v);
		v.clear();
		start = i + 1 + QString("</g>").length();
	}
	for(uint j=0;j<problems.size();j++)
		grp->insertItem(QString::number(j+1));
	if(grp->count()==1)
	{
		lblgrp->hide();
		grp->hide();
	}	
	if(grp->count()>1)
		grp->setCurrentItem(0);
	LoadGroup();
	return true ;
}
void welcome::start_time_load()
{
	if ( !QFile::exists(path_welcome+".stime"))
	{
		while(!WriteFile(path_welcome+".stime",GetCurrTime()))
		{
			int opt = QMessageBox::critical( this, "Error","Error Writing To File","Retry","Quit", 0, 0, 1 ) ;
			switch(opt)
			{
				case 0: break ;
				case 1: exit(1);
			}
		}
		s_time = ToSeconds(QTime :: currentTime());
	}	
	else
	{	
		QString stime ;
		while( (stime = ReadFile(path_welcome+".stime")) == "-1")
		{
			int opt = QMessageBox::critical( this, "Error","Error Reading File","Retry","Quit", 0, 0, 1 ) ;
			switch(opt)
			{
				case 0: break ;
				case 1: exit(1);
			}			
		}
		if(stime.length() < 3)
		{
			stime=GetCurrTime();
			WriteFile(path_welcome+".stime",stime);
		}
		QStringList s = QStringList :: split(" ",stime);
		QStringList::Iterator it = s.begin();
		s_time = (*it).toUInt() * 3600 ;
		it++;	
		s_time += (*it).toUInt() * 60 ;
		it++ ;	
		s_time += (*it).toUInt();
	}
}

//calculates ending time
void welcome::end_time_load()
{
	QString temp ;
	if ( QFile::exists(path_welcome+".ttime"))
	{		
		texists:
		while((temp=ReadFile(path_welcome+".ttime"))=="-1")
		{	
			int opt = QMessageBox::critical( this, "Error","Error Reading File","Retry","Quit", 0, 0, 1 ) ;
			switch(opt)
			{
				case 0: break ;
				case 1: exit(1);
			}
		}
		QStringList str = QStringList :: split(" ",temp);
		QStringList::Iterator it = str.begin();
		ulong t_time ;
		if((*it).toUInt()>3)
		{
			t_time = 3*3600 ;
			goto SKIP;
		}	
		else
			t_time = (*it).toUInt() * 3600 ;
		it++;
		if((*it).toUInt()>59)
			t_time += 59*60 ;
		else
			t_time += (*it).toUInt() * 60 ;
		it++ ;	
		if((*it).toUInt()>59)
			t_time += 59 ;
		else
			t_time += (*it).toUInt() ;
		SKIP:
		e_time = s_time + t_time ;
	}
	else
	{
		while(!QFile::exists(path_welcome+".ttime"))
		{	
			int opt = QMessageBox::critical( this, "Error","Required Resource DoesNot Exist","Retry","Quit", 0, 0, 1 ) ;
			switch(opt)
			{
				case 0: break ;
				case 1: exit(1);
			}
		}
		goto texists;
	}
}

QString welcome :: GetCurrTime()
{
	QTime ct = QTime :: currentTime();
	return QString::number(ct.hour())+" "+QString::number(ct.minute())+" "+QString::number(ct.second());
}
void welcome :: CreateDefaultOptions()
{
	QString options="0\n1\n1\n0\nRainbow";
	WriteFile(path_welcome+".options",options);
}
void welcome::init()
{
	Start->setAccel(Qt::Key_Return);
	pwait->hide();
	GetPath(path_welcome);
	bool ok =true;
	if(!QFile::exists(path_welcome+".serverip"))
	{
		QString sip ;
		
		repeat:
		
		sip = QInputDialog::getText ( "Server IP", "Enter Server's IP Address ", QLineEdit::Normal, "172.16.", &ok, this, 0 );
		
		if ( !ok || sip.isEmpty() )
		{
			MsgBoxC("Error","Please Enter a Valid IP Address");
			goto repeat ;
		}	
		WriteFile(path_welcome+".serverip",sip);
	}	
	ok=true;
	if(!QFile::exists(path_welcome+".port"))
	{
		QString port ;
		
		Repeat:
		
		port = QInputDialog::getText ( "Port", "Enter Port To Be Used", QLineEdit::Normal, "1234", &ok, this, 0 );
		
		if ( !ok || port.isEmpty() || port.toInt() < 1024 || port.toInt()>65536)
		{
			MsgBoxC("Error","Please Enter a Valid Port");
			goto Repeat ;
		}	
		WriteFile(path_welcome+".port",port);
	}	
	chmod(path_welcome+"client",S_IXUSR|S_IXGRP| S_IXOTH);
	if(!QFile::exists(path_welcome+".areg"))
		WriteFile(path_welcome+".areg","0");
	if(!QFile::exists(path_welcome+".sub"))
		WriteFile(path_welcome+".sub","1");
	if(!QFile::exists(path_welcome+".options"))
		CreateDefaultOptions();
	if ( IsRegistered() )
	{
		File->setEnabled(true);
		grp->setEnabled(true);	
		while(!LoadTeamName())
		{	
			int opt = QMessageBox::critical( this, "Error","Error Loading Team Name\n\n","Retry","Quit", 0, 0, 1);
			switch(opt)
			{
				case 0: break ;
				case 1: exit(1);
			}		
		}
		while(!LoadProbs())
		{	
			int opt = QMessageBox::critical( this, "Error","Error Loading Problem List","Retry","Quit", 0, 0, 1 ) ;
			switch(opt)
			{
				case 0: break ;
				case 1: exit(1);
			}		
		}
		
		while(!GetTotalTime())
		{	
			int opt = QMessageBox::critical( this, "Error","Error Connecting To Server ","Retry","Quit", 0, 0, 1 ) ;
			switch(opt)
			{
				case 0: break ;
				case 1: exit(1);
			}		
		}		
		start_time_load();
		end_time_load();
		QTime cct = QTime::currentTime();
		long current_time = ToSeconds(cct);
		long remaining_time = e_time - current_time ;
		if(remaining_time <= 0)
		{
			system(path_welcome+"./client 8 "+ReadFile(path_welcome+".team"));
			WriteFile(path_welcome+".sub","0");
		}
		QFile(path_welcome+".sub").remove();
		while(system(path_welcome+"./client 9 "+ReadFile(path_welcome+".team"))!=0)
		{	
			int opt = QMessageBox::critical( this, "Error","Error Connecting To Server ","Retry","Quit", 0, 0, 1 ) ;
			switch(opt)
			{
				case 0: break ;
				case 1: exit(1);
			}	
		}
		if(ReadFile(path_welcome+".sub")=="0")
			cansub=false;
	}
	else
	{
		File->setEnabled(false);
		grp->setEnabled(false);
	}
}

void welcome::Rules_clicked()
{
	rules* r = new rules(this);
	r->exec();
}
void welcome::Options_clicked()
{
	options* o = new options(this,0,Qt::WType_Dialog|Qt::WShowModal);
	o->show();
}

bool welcome :: GetProb ()
{
	system(path_welcome+"./client "+"4 "+ReadFile(path_welcome+".team")+" " + File->currentText() +" 0");
	if(!QFile::exists(path_welcome+File->currentText()+"p"))
		return false;
	else
		return true ;
}
void welcome :: CLOSE()
{
    if(QFile::exists(path_welcome+".egs"))
    {
	ide* i = new ide;
	i->show();
	this->close();
    }	
    else
    {	
	Start->show();
	Start->setText("<<[ GO ]>>");
	pwait->hide();
	int opt = QMessageBox::critical( this, "Error","Error Connecting To Server ","Retry","Cancel", 0, 0, 1 ) ;
	switch(opt)
	{
	    case 0: Start_clicked();
     	    case 1: return ;
	}
    }
}
void welcome::EXIT()
{
	if(QFile::exists(path_welcome+File->currentText()+"p"))
	{
	             	QFile(path_welcome+".egs").remove();
		if(pp!=NULL)
			delete pp;
		pp=new QProcess(this);
		pp->setWorkingDirectory(path_welcome);
		pp->addArgument ( "./client");
		pp->addArgument("11");
		pp->addArgument("0");
		connect( pp, SIGNAL( processExited()),this, SLOT(CLOSE()) );
		pp->start();
	}	
	else
	{
		Start->show();
		Start->setText("<<[ GO ]>>");
		pwait->hide();
		int opt = QMessageBox::critical( this, "Error","Error Connecting To Server ","Retry","Cancel", 0, 0, 1 ) ;
		switch(opt)
		{
			case 0: Start_clicked();
			case 1: return ;
		}
	}
}
void welcome :: SaveProblems()
{
	int prob_selected = File->currentText().toInt();
	QString probs;
	for(int i=0;i<grp->count();i++)
	{
		probs += "<g>\n";
		QString tgrp ;
		for(uint j=0;j<problems.at(i).size();j++)
		{
			if(problems.at(i).at(j) == prob_selected)
			{
				tgrp = QString::number(prob_selected)+"\n";
				goto nextgrp ;
			}
			else
				tgrp += QString::number(problems.at(i).at(j))+"\n" ;					}
		
		nextgrp :
				
		probs += tgrp+"</g>\n";	
	}
	WriteFile(path_welcome+".probs",probs);
}
void welcome :: Start_clicked()
{
	
    //check for registration
	if(!IsRegistered())
	{
		reg* r = new reg();
		r->exec();
		if(IsRegistered())
		{
			File->setEnabled(true);
			grp->setEnabled(true);
			Start->setText("<< GO >>");
			team->setText("Team : " + ReadFile(path_welcome+".team"));
			File->clear();
			while(!LoadProbs())
			{	
				int opt = QMessageBox::critical( this, "Error","Error Loading Problem List","Retry","Quit", 0, 0, 1 ) ;
				switch(opt)
				{
					case 0: break ;
					case 1: exit(1);
				}		
			}	
			while(!GetTotalTime())
			{	
				int opt = QMessageBox::critical( this, "Error","Error Connecting To Server ","Retry","Quit", 0, 0, 1 ) ;
				switch(opt)
				{
					case 0: break ;
					case 1: exit(1);
				}
			}	
		}	
		else
		{
			File->setEnabled(false);
			grp->setEnabled(false);
		}
	}	
	else
	{
		// ask user for confirmation for starting coding
		if(QMessageBox::warning(this, "Warning", "Are You Sure??", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
			return;
		Start->hide();
		pwait->show();
		SaveProblems();
		WriteFile(path_welcome+".p",File->currentText());
		bool first_time = !QFile::exists(path_welcome+File->currentText()+"p");
		QFile(path_welcome+File->currentText()+"p").remove();
		if(pp!=NULL)
			delete pp;
		pp=new QProcess(this);
		pp->setWorkingDirectory(path_welcome);
		pp->addArgument ( "./client");
		pp->addArgument("4");
		pp->addArgument(ReadFile(path_welcome+".team"));
		pp->addArgument(File->currentText());
		pp->addArgument("0");
		pp->addArgument(QString::number(first_time));
		connect( pp, SIGNAL( processExited()),this, SLOT(EXIT()) );
		pp->start();
	}
}


void welcome::grp_activated()
{
	LoadGroup();
}
