#include <qstring.h>
#include <qdir.h>
#include <pthread.h>
#include <qtimer.h>
#include <qmessagebox.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <qprocess.h>
#include <iostream>
#include <vector>
#include <qevent.h>
#include <qtextstream.h>
#include <qfile.h>
#include <qinputdialog.h> 
#include <qfiledialog.h> 
#include <algorithm>

#include "io.h"
#include "addrem.h"
#include "highlighter.h"

#define UPDATE_INTERVAL 1000

using namespace std ;

bool err=false;
int ci=-1;
uint li ;
uint casei = 0 ;
int tstime;
int s_time;
int timelimit ;
double currsubscore;
bool waskilled;
double score =0.0 ;
double total_score=0.0;
bool write_result = false;
int no_teams,no_probs;
bool perfect = true ;
bool no_wrong_cases=0;
bool compile_over;
bool binary_over;
pthread_t* pt =NULL;

struct Case
{
	QString input ;
	QString output ;
	double percentage ;
};

QString path ;
QString currteam;
QString currprob;
QString InputStartTag = "<i>" ;
QString InputEndTag = "</i>";
QString OutputStartTag = "<o>" ;
QString OutputEndTag = "</o>";
QString PercentageStartTag = "<p>";
QString PercentageEndTag = "</p>";
QString TimeLimitStartTag = "<tl>";
QString TimeLimitEndTag = "</tl>";
QTimer* stimer  ;
QTimer* t ;
QTimer* timer ;
QProcess* p = NULL ;
QProcess* s = NULL ;
FILE* fo ;

vector < pair < QString , vector < pair < QString,double> > > > submissions ;
vector <string> lines ;
vector <Case> vcases ;
vector< pair<QString ,double> > results ;
vector < vector <int> > problems;

 int admin::ToMSeconds(QTime t )
{
	return ( t.hour()*3600000 + t.minute()*60000 + t.second()*1000 + t.msec() );
}
void admin::MsgBox(QString title , QString msg)
{
	QMessageBox* m = new QMessageBox(title,msg,QMessageBox::Critical,QMessageBox::Ok,QMessageBox::NoButton, QMessageBox::NoButton);
	m->exec();
}
void admin::Check()
{
	if(p->isRunning() && (ToMSeconds(QTime::currentTime()) - s_time ) >= timelimit)
	{
	              system("kill " + QString::number(p->processIdentifier()));
		waskilled=true ;
		t->stop();
	}
}
void admin::SortResults()
{
	//Insertion Sort
	for ( uint i=1;i<results.size();i++)
	{
		pair<QString,double> temp =results.at(i);
		int j=i;
		while( j >= 1 && results.at(j-1).second < temp.second )
		{
			results.at(j)=results.at(j-1);
			j--;
		}	
		results.at(j)=temp;
	}	
}

double admin :: MinScore(int pname)
{
	LoadProblems();
	int gindex,min=100;
	for(uint i=0;i<problems.size();i++)
	{
		for(uint j=0;j<problems.at(i).size();j++)
		{
			if(problems.at(i).at(j)== pname)
			{
				gindex=i;
				min =*min_element(problems.at(i).begin(),problems.at(i).end());
				goto breaktwice;
			}
		}		
	}

	breaktwice:
	
	QFile ftemp (path + "/ttime");
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
	return min*(0.3+(0.7*tt*tt)/((10.0*tt*tt)+tt*tt)) ;
}

void admin::AfterExit()
{
	int etime ;
	QString poutput,e ;
	etime=ToMSeconds(QTime::currentTime());
	bool correct=true;
	double casescore=0.0;
	if(err)
		goto NEXT;
	if(waskilled == true )
	{
		perfect = false;
		no_wrong_cases++;
		correct=false;
		OP("\nProcess Timed Out , .'. Killed :-(");
		waskilled = false ;
		goto next ;
	}	
	if(!p->normalExit ())
	{
		perfect=false;
		no_wrong_cases++;
		correct=false;
		OP("\nProcess Exited With An Error :-(");
		goto next ;
	}	
	poutput = QString(p->readStdout());
	if(poutput.simplifyWhiteSpace() ==  vcases.at(casei).output.simplifyWhiteSpace() )
	{
		OP("Correct :-)");
		casescore = currsubscore * vcases.at(casei).percentage / 100.0;
		score += casescore;
	}
	else
	{
		perfect=false;
		correct=false;
		no_wrong_cases++;
		OP("Incorrect :(");
		e = "\n\nInput : \n\n" + vcases.at(casei).input.stripWhiteSpace();
		e += "\n\nProgram Output : \n\n" + poutput.stripWhiteSpace() ;
		e += "\n\nRequired Output : \n\n" + vcases.at(casei).output.stripWhiteSpace()+"\n";
		if( chkip->isChecked())
			OP(e);
		else
			TestOP->insert(e);
	}
	next :
	OP("\nScore : " + QString::number(casescore)) ;
	if(correct)
		OP("\nTimeTaken : "+QString::number(etime-s_time)+" msecs");
	NEXT :
	casei ++ ;
	pbc->setProgress(casei);	
	if(casei<=vcases.size()-1)
		NextTest() ;	
	else
	{
		if(!perfect)
		{
			OP("\n\n");
			OP("Output For "+QString::number(no_wrong_cases));
			if(no_wrong_cases==1)
				OP(" Test Case was WRONG !!!\n\n");
			else
				OP(" Test Cases were WRONG !!!\n\n");
			double prev_score=score ;
			score = score*MinScore(currprob.toInt())/currsubscore ;
			OP("Total Score For "+currprob+" Point Problem : " + QString::number(score) +" ( Instead Of "+QString::number(prev_score)+" )"+"\n");
		}	
		else
			OP("\n\nTotal Score For "+currprob+" Point Problem : " + QString::number(score) +"\n");
		
		vcases.clear();
		casei=0;
		perfect=true ;
		no_wrong_cases=0;
		total_score += score ;
		score = 0.0 ;
		li++ ;
		pbo->setProgress(li);
		if(li <= lines.size()-1) 
			Process ();
		else
		{
			pbt->setProgress(pbt->totalSteps());
			pbp->setProgress(pbp->totalSteps());
			OP("\n......................................................................\n\n");
			OP("Total Score Of Team "+currteam +" : "+QString::number(total_score)+"\n\n");
			if(write_result   )
			{	
				pair<QString,double> temppair ;
				temppair.first=currteam;
				temppair.second=total_score;
				total_score = 0.0 ;	
				results.push_back(temppair);
				SortResults();
			}
			lines.clear();
			SortResults();
			if(write_result && chkresult->isChecked())
			{
				TestOP->insert("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
				TestOP->insert("Result : \n\n");
				for(uint i=0;i<results.size();i++)
				{
					//QString sres = results.at(i).first +" " + QString::number(results.at(i).second)+"\n";
					QString team = results.at(i).first;
					team += "(" +GetCourse(team) +","+GetCollege(team)+")";
				QString tres ="<"+QString::number(i+1)+">   "+ QString( "%1 %2\n" ).arg(team,-12).arg( QString::number(results.at(i).second),-10);
					TestOP->insert(tres);
					AppendFile(path+"result",tres);
				}
				TestOP->insert("\n");
				TestOP->setEnabled(true);
			}	
			int tet = ToMSeconds(QTime::currentTime( ));
			TestOP->insert("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
			TestOP->insert("Total Testing Time : " + QString::number (double(tet-tstime)/1000.0) + " secs(approx)\n\n");
			TestOP->setEnabled(true);
			Test->setEnabled(true);
			Problem->setEnabled(true);
			Team->setEnabled(true);
			Refresh->setEnabled(true);
			lblc->hide();
			lblp->hide();
			lblo->hide();
			lblt->hide();
			pbc->hide();
			pbp->hide();
			pbo->hide();
			pbt->hide();
		}
	}
}
void admin ::OP (QString s)
{
	TestOP->insert(s);
	if(write_result)
		AppendFile(QDir::homeDirPath()+"/users/"+currteam+"/result",s);
}
void admin :: LaunchFinished()
{
	OP("\n\nTest Case " + QString::number(casei+1) + " : ");
	t->start(timelimit,false);
	s_time=ToMSeconds(QTime::currentTime());
}
void admin::NextTest()
{
	QString arg=QDir::homeDirPath()+"/users/"+currteam+"/"+currprob;
	if(p!=NULL)
		delete p;
	p = new QProcess (this);
	p->clearArguments();
	p->addArgument(arg);
	connect(p,SIGNAL(launchFinished ()),this,SLOT( LaunchFinished()));
	connect( p, SIGNAL(processExited()),this, SLOT(AfterExit( )) );
	if(t!=NULL)
		delete t;
	t = new QTimer   (this);
	connect ( t,SIGNAL(timeout()),SLOT(Check()));
	err=false;
	p->launch( vcases.at(casei).input);
}
void admin::closeEvent(QCloseEvent* ce)
{
	if(QMessageBox::warning(this, "Warning", "Are You Sure U Want To Close Admin??", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
			return;
	if(s!=NULL && s->isRunning())
		system("kill "+ QString::number(s->processIdentifier()));
	ce->accept();
}
void admin::Push()
{
	QString cases = ReadFile(path + currprob+"test") ;
	int tlsi = cases.find(TimeLimitStartTag,0) ;
	tlsi += TimeLimitStartTag.length();
	int tlei =  cases.find(TimeLimitEndTag,tlsi)-1 ;
	QString tlimit=cases.mid(tlsi,tlei-tlsi+1);
	tlimit=tlimit.simplifyWhiteSpace();
	timelimit = tlimit.toInt();
	int start = 0 ;
	
	int input_start_index ;
	Case t ;
	while ( ( input_start_index = cases.find(InputStartTag,start) ) != -1 )
	{
		input_start_index +=InputStartTag.length() ;
		int input_end_index = cases.find(InputEndTag,input_start_index)-1;
		
		QString i = cases.mid(input_start_index,input_end_index-input_start_index+1);
		t.input = i.stripWhiteSpace();
		
		start = input_end_index + 1 + InputEndTag.length();
		
		int output_start_index = cases.find(OutputStartTag,start) + OutputStartTag.length() ;
		int output_end_index = cases.find(OutputEndTag,output_start_index)-1;
		
		QString o = cases.mid(output_start_index,output_end_index-output_start_index+1);
		t.output = o.stripWhiteSpace();
		
		start = output_end_index + 1 + OutputEndTag.length();
		
		int percentage_start_index = cases.find(PercentageStartTag,start) + PercentageStartTag.length() ;
		int percentage_end_index  = cases.find(PercentageEndTag,percentage_start_index) - 1 ;
		
		QString per = cases.mid(percentage_start_index,percentage_end_index-percentage_start_index+1) ;
		QString percentage = per.simplifyWhiteSpace();
		t.percentage=percentage.toDouble() ;
		
		start = percentage_end_index + 1 + PercentageEndTag.length();
		vcases.push_back(t);
	}
}

void admin::Process()
{
	QString request = lines.at(li);
	int j = request.find(" ",0) ;
	QString prevteam = currteam ;
	currteam = request.mid(0,j);
	if(li==0)
	{
		vector <QString> v;
		for ( uint i =0 ; i < submissions.size();i++)
			if ( submissions.at(i).first == currteam )
				for ( uint j = 0; j < submissions.at(i).second.size();j++)
					v.push_back(submissions.at(i).second.at(j).first);
		
		pbp->setTotalSteps(v.size());
		pbp-> setProgress(0);
		
		pbt->setTotalSteps(no_teams);
		pbt-> setProgress(0);
	}
	else
	{
		if(currteam!=prevteam)
		{
			pbt->setProgress(pbt->progress()+1);
			vector <QString> v;
			for ( uint i =0 ; i < submissions.size();i++)
				if ( submissions.at(i).first == currteam )
					for ( uint j = 0; j < submissions.at(i).second.size();j++)
						v.push_back(submissions.at(i).second.at(j).first);
			pbp->setTotalSteps(v.size());
			pbp-> setProgress(0);
		}	
		else
			pbp->setProgress(pbp->progress()+1);
	}
	bool change_of_team=false;
	
	if(currteam!=prevteam || li==0)
		change_of_team = true ;
	if(currteam!=prevteam && li !=0)
	{
		QString t=currteam ;
		currteam=prevteam;
		OP("\n......................................................................\n\n");
		OP("Total Score Of Team "+prevteam +" : "+QString::number(total_score)+"\n");
		currteam=t;
		if(write_result)
		{	
			pair<QString,double> temppair ;
			temppair.first=prevteam;
			temppair.second=total_score;
			results.push_back(temppair);
			SortResults();
		}		
		total_score = 0.0 ;	
	}	
	int k = request.find(" ",j+1) ;
	currprob = request.mid(j+1,k-j-1);
	int m = request.find("\n",k+1);
	QString ss=request.mid(k+1,m-k-1);
	currsubscore=ss.toDouble();
	Push();
	if(change_of_team)
	{
		if(li)
			TestOP->insert("\n*********************************************\n");
		TestOP->insert("\nTeam : " + currteam +"\n");
	}	
	else
		OP("\n------------------------------------------------------------\n");
	OP("\nTesting "+currprob + " Point Problem ......") ;
	OP("\nSubmitted For : " + QString::number(currsubscore) );
	OP("\nTimeLimit: " + QString::number(timelimit) + " msecs");
	pbc->setTotalSteps(vcases.size());
	pbc-> setProgress(0);
	NextTest();
}

QString admin :: ReadFile ( QString fname ) 
{
	QString data="" ;
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
		data="-1";
	return data ;
}
void admin::GetPath()
{
	path = QDir::homeDirPath()+"/.files/";
}
void admin::init()
{
	GetPath();
	Stop->hide();
	if(!QFile::exists(path+"ttime"))
	{
		QString ttime;
		bool ok=true;
		
		Repeat1:
		
		ttime = QInputDialog::getText ( "Total Time", "Enter Total Time", QLineEdit::Normal, "02 00 00", &ok, this, 0 );
	    
		if ( !ok || ttime.isEmpty() )
		{
			MsgBox("Error","Please Enter a Valid Total Time");
			goto Repeat1 ;
	    }	
	    WriteFile(path+"ttime",ttime);
	}
	bool ok=true;
	if(!QFile::exists(path+".port"))
	{
	    QString port ;
		
	    Repeat:
		
	    port = QInputDialog::getText ( "Port", "Enter Port To Be Used", QLineEdit::Normal, "1234", &ok, this, 0 );
	    
	    if ( !ok || port.isEmpty() || port.toInt() < 1024 || port.toInt()>65536)
	    {
		MsgBox("Error","Please Enter a Valid Port");
		goto Repeat ;
	    }	
	    WriteFile(path+".port",port);
	}	
	if( ! QFile::exists(path+".user_list") ) WriteFile(path+".user_list","");
	if( ! QFile::exists(path+".user_timeup") ) WriteFile(path+".user_timeup","");
	if( ! QFile::exists(path+"masterlog.txt") ) WriteFile(path+"masterlog.txt","");	
	if( ! QFile::exists(path+"log.txt") ) WriteFile(path+"log.txt","");	
	if( ! QFile::exists(path+"queue") ) WriteFile(path+"queue","");	
	if( ! QFile::exists(path+"probs") ) WriteFile(path+"probs","");	
	
	
	ProbText->setTabStopWidth(20);
	Code->setTabStopWidth(20);
}

void admin::OnExit()
{
	Start->show();
	Stop->hide();
	//stimer->stop();
	QTime ctime  = QTime::currentTime();
	QString msg = "Stopped ( " + QString::number(ctime.hour())+":" ;
	msg += QString::number(ctime.minute()) + ":" + QString::number(ctime.second())+" )\n";
	AppendFile(path+"log.txt",msg);
	Update();
}

QString admin :: GetCourse(QString team)
{
	QString tpath = QDir::homeDirPath()+"/users/"+team+"/";
	return ReadFile(tpath+"course") ;
}
QString admin :: GetCollege(QString team)
{
	QString tpath = QDir::homeDirPath()+"/users/"+team+"/";
	return ReadFile(tpath+"college") ;
}
void admin :: Update()
{
	if( ! QFile::exists(path+".user_list") ) WriteFile(path+".user_list","");
	if( ! QFile::exists(path+".user_timeup") ) WriteFile(path+".user_timeup","");
	if( ! QFile::exists(path+"masterlog.txt") ) WriteFile(path+"masterlog.txt","");	
	if( ! QFile::exists(path+"log.txt") ) WriteFile(path+"log.txt","");	
	if( ! QFile::exists(path+"queue") ) WriteFile(path+"queue","");	
	QString temp = ReadFile(path+"queue");
	SubQ->setText("Submissions : \n\n"+temp);
	Log->setText("Server Log : \n\n"+ReadFile(path+"log.txt"));
	QString teamlist ; 
	QStringList sl = QStringList::split("\n",ReadFile(path+".user_list"));
	for(QStringList::Iterator i=sl.begin();i!=sl.end();i++)
		teamlist += QString( "%1%2\n" ).arg( (*i),-15).arg("( " +GetCourse(*i) +" , "+GetCollege(*i)+" )",-30);
	UList->setText("Registered Teams : \n\n"+teamlist);
	TUpList->setText("Time Up : \n\n"+ ReadFile(path+".user_timeup"));
	
}
void admin::Start_clicked()
{
	bool ok=true;
	if(!QFile::exists(path+".port"))
	{
	    QString port ;
		
	    Repeat:
		
	    port = QInputDialog::getText ( "Port", "Enter Port To Be Used", QLineEdit::Normal, "1234", &ok, this, 0 );
	    
	    if ( !ok || port.isEmpty() || port.toInt() < 1024 || port.toInt()>65536)
	    {
		MsgBox("Error","Please Enter a Valid Port");
		goto Repeat ;
	    }	
	    WriteFile(path+".port",port);
	}
	if(stimer!=NULL)
		delete stimer;
	stimer = new QTimer (this);
	connect ( stimer,SIGNAL(timeout()),SLOT(Update()));
	if(s!=NULL)
		delete s;
	s=new QProcess(this);
	s->addArgument ( QDir::homeDirPath()+"/Server/./server" );
	connect( s, SIGNAL( processExited()),this, SLOT(OnExit()) );
	if(s->start())
	{
		stimer->start(UPDATE_INTERVAL,false);
		Update();
		Start->hide();
		Stop->show();
		
	}
	else
		MsgBox("Error","Error Starting Server");
}

void admin::Stop_clicked()
{
	if(s!=NULL && s->isRunning())
	{
		if(QMessageBox::warning(this, "Warning", "Are You Sure u want to stop the server ??", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
			return;
		system("kill -s SIGKILL "+ QString::number(s->processIdentifier()));
	}	
}
bool admin :: WriteFile ( QString fname , QString data ) 
{
	QFile f ( fname) ;
	
	if ( f.open( IO_WriteOnly) )
	{
		QTextStream s ( & f);
		s<<data;f.flush();
		f.close();
		return true ;
	}
	else
		return false;
}
void admin::Test_clicked()
{
	TestOP->setEnabled(false);
	QString def_keywords = "Correct darkGreen\nIncorrect red\nKilled red\nError red\nResult darkBlue\n";
	WriteFile(path+".skeywords",def_keywords);
	for(int i=0;i<Team->count();i++)
	{
		if(Team->text(i) != "All")
			AppendFile(path+".skeywords",Team->text(i)+ " " + "darkMagenta\n");
	}	
	for(int i=0;i<Problem->count();i++)
	{
		if(Problem->text(i) != "All")
			AppendFile(path+".skeywords",Problem->text(i)+ " " + "darkCyan\n");
	}	
	if(q!=NULL)
		delete q;
	q = new SyntaxHighlighter(path+".skeywords",TestOP);
	total_score=0.0;
	write_result=false;
	Test->setEnabled(false);
	Problem->setEnabled(false);
	Team->setEnabled(false);
	Refresh->setEnabled(false);
	if(Team->count() == 0)
		return ;
			
	TestOP->clear();
	tstime=ToMSeconds(QTime::currentTime());
	results.clear();
	if ( Team->currentText() != "All" && Problem->currentText() != "All" )
	{
		no_teams=1;
		for ( uint i = 0 ; i < submissions.size() ; i ++)
		{
			if(submissions.at(i).first == Team->currentText())
			{
				for ( uint j = 0 ; j < submissions.at(i).second.size();j++)
				{
					if (submissions.at(i).second.at(j).first == Problem->currentText())
						lines.push_back( submissions.at(i).first + " " + submissions.at(i).second.at(j).first + " " + QString::number(submissions.at(i).second.at(j).second));
				}	
			}	
		}
	}
	else if ( (Team->currentText() == "All" || Team->count ()==1) &&( Problem->currentText() == "All"   ))
	{
		if(Team->currentText()=="All")
			no_teams = Team->count()-1;
		else
			no_teams=1;
		for(int i=0;i<Team->count();i++)
		{
			QString temp = QDir::homeDirPath()+"/users/"+Team->text(i)+"/result";
			QFile(temp).remove();
		}		
		QFile(path+"result").remove();
		write_result=true ;
		for ( uint i = 0 ; i < submissions.size() ; i ++)
		{
			for ( uint j = 0 ; j < submissions.at(i).second.size();j++)
			{
				lines.push_back( submissions.at(i).first + " " + submissions.at(i).second.at(j).first + " " + QString::number(submissions.at(i).second.at(j).second));
			}	
		}
	}
	else if ( (Team->currentText() == "All" || Team->count ()==1) && (Problem->currentText() != "All" ))
	{
		if(Team->currentText()=="All")
			no_teams = Team->count()-1;
		else
			no_teams=1;
		for ( uint i = 0 ; i < submissions.size() ; i ++)
		{
			for ( uint j = 0 ; j < submissions.at(i).second.size();j++)
			{
				if (submissions.at(i).second.at(j).first == Problem->currentText())
					lines.push_back( submissions.at(i).first + " " + submissions.at(i).second.at(j).first + " " + QString::number(submissions.at(i).second.at(j).second));
			}	
		}
	}
	else if ( Team->currentText() != "All" && Problem->currentText() == "All" )
	{
		no_teams=1;
		for ( uint i = 0 ; i < submissions.size() ; i ++)
		{
			if(submissions.at(i).first == Team->currentText())
			{
				for ( uint j = 0 ; j < submissions.at(i).second.size();j++)
				{
					lines.push_back( submissions.at(i).first + " " + submissions.at(i).second.at(j).first + " " + QString::number(submissions.at(i).second.at(j).second));
				}	
			}	
		}
	}
	li = 0 ;
	lblc->show();
	lblp->show();
	lblo->show();
	lblt->show();
	
	pbc->show();
	pbp->show();
	pbo->show();
	pbt->show();
	
	pbo->setTotalSteps(lines.size());
	pbo-> setProgress(0);
	Process();
}

void admin::PopulateCombo(QComboBox*& master,QComboBox*& cmb)
{
	cmb->clear();
	vector <QString> v ;
	for ( uint i =0 ; i < submissions.size();i++)
	{
		if ( submissions.at(i).first == master->currentText() )
		{
			for ( uint j = 0; j < submissions.at(i).second.size();j++)
			{
				v.push_back(submissions.at(i).second.at(j).first);
			}
		}
	}	
	sort(v.begin(),v.end());
	for ( uint i = 0; i < v.size();i++)
		cmb->insertItem(v.at(i));
	if(master == Team && (cmb->count() > 1 || master->currentText()=="All"))
		cmb->insertItem("All");
}
bool admin :: AppendFile(QString fname,QString s)
{
	QFile f(fname);
	if(f.open(IO_WriteOnly | IO_Append))
	{
		QTextStream stream (&f);
		stream << s;
		f.close();
		return true ;
	}
	else
		return false;
}
void admin::Team_activated()
{
    PopulateCombo(Team,Problem);
}
void admin::LoadSubmissions()
{
	submissions.clear();
	QString q = ReadFile(path+"queue");
	QStringList sl = QStringList::split("\n",q);
	QStringList::Iterator i ;
	for( i = sl.begin();i!=sl.end();i++)
	{
		QStringList submission = QStringList::split(" ",*i);
		QStringList::iterator j = submission.begin();
		QString teamname = *j;
		j++;
		QString probname = *j;
		j++;
		QString sscore = *j;
		double score = sscore.toDouble();
		
		pair <QString,double> p;
		vector < pair <QString,double> > v ;
		pair <QString , vector < pair <QString,double> > > tp ;
		
		for ( uint j = 0 ; j < submissions.size(); j++)
		{
			if( teamname == submissions.at(j).first )
			{
				pair <QString,double> p;
				for ( uint k = 0 ; k<submissions.at(j).second.size() ; k ++)
				{
					if ( submissions.at(j).second.at(k).first == probname )
					{
						submissions.at(j).second.at(k).second = score ;
						goto next ;
					}
				}
				p.first = probname ;
				p.second = score;
				
				submissions.at(j).second.push_back(p);
				
				goto next ;
			}
		}
			// team's first submission
		
		p.first = probname ;
		p.second = score;
				
		v.push_back(p);
			
		tp.first = teamname ;
		tp.second = v ;
			
		submissions.push_back(tp);
		
		next : ;
	}
}
void admin::LoadCode()
{
	if(!QFile::exists(path+Prob->currentText()+".cpp"))
	{
		WriteFile(path+Prob->currentText()+".cpp","");
		checkBox2->setChecked(false);
	}	
	Code->setText(ReadFile(path+Prob->currentText()+".cpp"));
}
void admin::LoadGroup()
{
	Prob->clear();
	if(grp->count()==0)
		return ;
	int gindex = grp->currentText().toInt()-1;
	for(uint i=0;i<problems.at(gindex).size();i++)
		Prob->insertItem(QString::number(problems.at(gindex).at(i)));
	if(ci >= 0 && Prob->count () >= 1)
		Prob->setCurrentItem(ci);
}
void admin :: LoadProblems()
{
	QString probs = ReadFile(path+"probs");
	int start = 0 ,i ,e;
	problems.clear();
	while ( ( i = probs.find("<g>",start) ) != -1 )
	{
	    i +=QString("<g>").length() ;
	    e = probs.find("</g>",i)-1;
	    QString group = probs.mid(i,e-i+1).simplifyWhiteSpace();
	    QStringList sl = QStringList::split(" ",group);
	    vector<int> v;
	    for(QStringList::Iterator k=sl.begin();k!=sl.end();k++)
		v.push_back((*k).toInt());
	    problems.push_back(v);
	    start = i + 1 + QString("</g>").length();
	}
}
void admin::Tab_selected( const QString & tab_title )
{
	if (tab_title == "Codes")
	{
		LoadSubmissions();
		Teamcmb->clear();
		for ( uint i =0 ; i < submissions.size();i++)
			Teamcmb->insertItem(submissions.at(i).first) ;
		PopulateCombo(Teamcmb,Problemcmb); 
		LoadUserCode();
	}
	if (tab_title == "Test")
	{
		LoadSubmissions();
		Team->clear();
		for ( uint i =0 ; i < submissions.size();i++)
			Team->insertItem(submissions.at(i).first) ;
		if(Team->count() > 1 )
			Team->insertItem("All");
		PopulateCombo(Team,Problem); 
		if(Team->count ()==0)
			Test->hide();
		else
			Test->show();
		lblc->hide();
		lblp->hide();
		lblo->hide();
		lblt->hide();
		
		pbc->hide();
		pbp->hide();
		pbo->hide();
		pbt->hide();
		
	}
	if(tab_title== "Time")
	{
		Team1->clear();
		Team1->insertStringList(QStringList::split("\n",ReadFile(path+".user_list")));
		Team2->clear();
		Team2->insertStringList(QStringList::split("\n",ReadFile(path+".user_list")));
		prob->clear();
		LoadProblems();
		vector<int> v;
		for(uint i=0;i<problems.size();i++)
			for(uint j=0;j<problems.at(i).size();j++)
				v.push_back(problems.at(i).at(j));
		sort(v.begin(),v.end());
		QStringList  tsl;
		for(uint i=0;i<v.size();i++)
			tsl << QString::number(v.at(i)) ;
		prob->insertStringList(tsl);
		if(prob->count() >= 0 )
			prob->setCurrentItem(0);
		Updatett();
		Updatest();
		Loaditt();
	}
	if(tab_title=="Problems")
	{
		LoadProblems();
		grp->clear();
		for(uint j=0;j<problems.size();j++)
			grp->insertItem(QString::number(j+1));
		if(grp->count()>0)
			grp->setCurrentItem(0);
		LoadGroup();
		LoadProb();
		LoadCode();
	}
	if(tab_title=="Rules")
	{
		if(!QFile::exists(path+"rules"))
			WriteFile(path+"rules","");
		txtrules->setText(ReadFile(path+"rules"));
	}
}

void admin :: LoadProb()
{
	if(!QFile::exists(path+Prob->currentText()+".txt") )
	{
		WriteFile(path+Prob->currentText()+".txt","");
		checkBox1->setChecked(false);
	}	
	ProbText->setText(ReadFile(path+Prob->currentText()+".txt"));
}
void admin::Updatest()
{
	QString st = ReadFile(QDir::homeDirPath()+"/users/"+Team2->currentText()+"/"+prob->currentText()+"st") ;
	if(st=="-1")
	{
		add2->setEnabled(false);
		sub2->setEnabled(false);
		return ;
	}
	else
		add2->setEnabled(true);
		sub2->setEnabled(true);
	QStringList t = QStringList::split(" ",st);
	QStringList::Iterator i = t.begin();
	h2->setText(*i);
	i++;
	m2->setText(*i);
	i++;
	s2->setText(*i);
}
void admin::Updatett()
{
	QString tt = ReadFile(QDir::homeDirPath()+"/users/"+Team1->currentText()+"/ttime") ;
	if(tt=="-1")
		return ;
	QStringList t = QStringList::split(" ",tt);
	QStringList::Iterator i = t.begin();
	h1->setText(*i);
	i++;
	m1->setText(*i);
	i++;
	s1->setText(*i);
}
void admin::Refresh_clicked()
{
	LoadSubmissions();
	Team->clear();
	for ( uint i =0 ; i < submissions.size();i++)
		Team->insertItem(submissions.at(i).first) ;
	if(Team->count() > 1 )
		Team->insertItem("All");
	PopulateCombo(Team,Problem);
	if(Team->count ()==0)
			Test->hide();
		else
			Test->show();
}

void admin::Team1_activated()
{
	Updatett();
}


void admin::Team2_activated()
{
	Updatest();
}

void admin::refresh1_clicked()
{
	Team1->clear();
	Team1->insertStringList(QStringList::split("\n",ReadFile(path+".user_list")));
	Updatett();
}
void admin :: Loaditt()
{
	QString temp = ReadFile(path+"ttime");
	QStringList itt = QStringList::split(" ",temp);
	QStringList::Iterator i = itt.begin();
	shour3->setValue((*i).toInt());
	i++;
	sminute3->setValue((*i).toInt());
	i++;
	ssec3->setValue((*i).toInt());
}
void admin :: AddSub1(char sign )
{
	QString tt = ReadFile(QDir::homeDirPath()+"/users/"+Team1->currentText()+"/ttime") ;
	QStringList t = QStringList::split(" ",tt);
	QStringList::Iterator i = t.begin();
	int time = (*i).toInt()*3600;
	i++;
	time += (*i).toInt()*60 ;
	i++;
	time += (*i).toInt();
    
	if(sign == '+')
		time += shour1->value()*3600 + sminute1->value()*60 + ssec1->value();
	else
		time -= shour1->value()*3600 + sminute1->value()*60 + ssec1->value();
	
	int h=time/3600;
	int m = (time - h*3600)/60 ;
	int s = (time - h*3600 - m*60 );
	
	QString ntt = QString::number(h) + " " ;
	ntt += QString::number(m) + " ";
	ntt += QString::number(s) ;
    
	WriteFile(QDir::homeDirPath()+"/users/"+Team1->currentText()+"/ttime",ntt);
	Updatett();
}
void admin::add1_clicked()
{
	AddSub1('+');
}

void admin::sub1_clicked()
{
	AddSub1('-');
}

void admin::refresh2_clicked()
{
	Team2->clear();
	prob->clear();
	Team2->insertStringList(QStringList::split("\n",ReadFile(path+".user_list")));
	prob->insertStringList(QStringList::split("\n",ReadFile(path+"probs")));
	if(prob->count () >= 1)
	{
		prob->removeItem(0);
		prob->setCurrentItem(0);
	}	
	Updatest();	
}

void admin :: AddSub2(char sign)
{
	QString st = ReadFile(QDir::homeDirPath()+"/users/"+Team2->currentText()+"/"+prob->currentText()+"st") ;
	QStringList t = QStringList::split(" ",st);
	QStringList::Iterator i = t.begin();
	
	int time = (*i).toInt()*3600;
	i++;
	time += (*i).toInt()*60 ;
	i++;
	time += (*i).toInt();
    
	if(sign == '+')
		time += shour2->value()*3600 + sminute2->value()*60 + ssec2->value();
	else
		time -= shour2->value()*3600 + sminute2->value()*60 + ssec2->value();
	
	int h=time/3600;
	int m = (time - h*3600)/60 ;
	int s = (time - h*3600 - m*60 );
	
	QString nst = QString::number(h) + " " ;
	nst += QString::number(m) + " ";
	nst += QString::number(s) ;
	
	WriteFile(QDir::homeDirPath()+"/users/"+Team2->currentText()+"/"+prob->currentText()+"st",nst);
	Updatest();
}	
void admin::prob_activated()
{
	Updatest();
}


void admin::add2_clicked()
{
	AddSub2('+');
}


void admin::sub2_clicked()
{
	AddSub2('-');
}

void admin::Apply_clicked()
{
	QString itt = QString :: number(shour3->value())+ " ";
	itt += QString::number(sminute3->value())+ " ";
	itt += QString ::number(ssec3->value());
	WriteFile(path+"ttime",itt);
	Apply->setEnabled(false);
}

void admin::Prob_activated()
{
	LoadProb();
	LoadCode();
}

void admin::checkBox1_toggled( bool state )
{
	if(state)
	{
		ProbText->setReadOnly(true);
		Load1->setEnabled(false);
	}	
	else
	{
		ProbText->setReadOnly(false);
		Load1->setEnabled(true);
	}	
}


void admin::ProbText_textChanged()
{
	if(Prob->count()!=0)
		WriteFile(path+Prob->currentText()+".txt",ProbText->text());
}

void admin::Load1_clicked()
{
	if(QMessageBox::warning(this, "Warning", "Are You Sure u want to delete current question and load a new question from an existing file?? ", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
			return;
	QString fname = QFileDialog::getOpenFileName (QDir::homeDirPath() , "*.txt *.doc *.cpp *.c *.rtf ",this,0,"Open Problem From A File");
	if(fname!=QString::null)
		ProbText->setText(ReadFile(fname));
}

void admin::checkBox2_toggled( bool state )
{
	if(state)
	{
		Code->setReadOnly(true);
		Load2->setEnabled(false);
	}	
	else
	{
		Code->setReadOnly(false);
		Load2->setEnabled(true);
	}	
}

void admin::Load2_clicked()
{
	if(QMessageBox::warning(this, "Warning", "Are You Sure u want to delete current code and load a new code from an existing file??? ", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
			return;
	QString fname = QFileDialog::getOpenFileName (QDir::homeDirPath() , "*.txt *.doc *.cpp *.c *.rtf ",this,0,"Open Code From A File");
	if(fname!=QString::null)
		Code->setText(ReadFile(fname));
}
void admin::Add_clicked()
{
	if(grp->count()==0)
		Add_2_clicked();
	bool ok=true;
	QString pname = QString::number(QInputDialog::getInteger ( "Points", "Enter No Of Points : ",100, 50, 9999, 25, &ok, this ));
	if(!ok)
		return ;
	
	for(uint i=0;i<problems.size();i++)
	{
		for(uint j=0;j<problems.at(i).size();j++)
		{
			if(problems.at(i).at(j)==pname.toInt())
			{
				MsgBox("Error","Problem Already Exists");
				return ;
			}	
		}
	}
	int gindex =grp->currentText().toInt()-1; 
	problems.at(gindex).push_back(pname.toInt());
	sort(problems.at(gindex).begin(),problems.at(gindex).end());
	uint pindex=0;
	for(;pindex<problems.at(gindex).size();pindex++)
	{
		if(problems.at(gindex).at(pindex)==pname.toInt())
			break ;
	}
	
	QString newprobs ;
	for(uint i=0;i<problems.size();i++)
	{
		newprobs +="<g>\n";
		for(uint j=0;j<problems.at(i).size();j++)
			newprobs += QString::number(problems.at(i).at(j))+"\n" ;
		newprobs += "</g>\n";
	}
	WriteFile(path+"probs",newprobs);
	WriteFile(path+pname+".txt","");
	WriteFile(path+pname+".cpp","");
	LoadProblems();
	ci=pindex;
	LoadGroup();
	ci=-1; 
	LoadProb();
	LoadCode();
	checkBox1->setChecked(false);
	checkBox2->setChecked(false);
	ProbText->clear();
	Code->clear();
}
void admin :: OnCExit()
{
	if(!QFile::exists(path+"a.out"))
	{
		MsgBox("Error","The Code Didn't Compile Successfully");
		Cases->setText("Edit Test Cases");
		return ;
	}
}
void admin::Cases_clicked()
{
	if(Prob->count() == 0)
		return ;
	WriteFile(path+".p",Prob->currentText());
	WriteFile(path+".mode","cases");
	io* i = new io();
	i->exec();
}

void admin::Code_textChanged()
{
	WriteFile(path+Prob->currentText()+".cpp",Code->text());
}

void admin::checkBox3_toggled( bool state )
{
	if(state)
		txtrules->setReadOnly(true);
	else
		txtrules->setReadOnly(false);
}


void admin::txtrules_textChanged()
{
	WriteFile(path+"rules",txtrules->text());
}


void admin::Load3_clicked()
{
	if(QMessageBox::warning(this, "Warning", "Are You Sure ??? ", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
			return;
	QString fname = QFileDialog::getOpenFileName (QDir::homeDirPath() , "*.txt *.doc *.cpp *.c *.rtf ",this,0,"Open Rules From A File");
	if(fname!=QString::null)
		txtrules->setText(ReadFile(fname));
}


void admin::Rem_clicked()
{
	if(Prob->count()==0)
		return ;
	QString pname = Prob->currentText();
	if(QMessageBox::warning(this, "Warning", "Are You Sure ??? ", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
		return;
	
	int gindex =grp->currentText().toInt()-1; 
	for(uint i=0;i<problems.at(gindex).size();i++)
	{
		if(problems.at(gindex).at(i)==pname.toInt())
		{
			problems.at(gindex).erase(problems.at(gindex).begin()+i);
			Prob->setCurrentItem(0);
		}	
	}
	QString newprobs ;
	for(uint i=0;i<problems.size();i++)
	{
		newprobs +="<g>\n";
		for(uint j=0;j<problems.at(i).size();j++)
			newprobs += QString::number(problems.at(i).at(j))+"\n" ;
		newprobs += "</g>\n";
	}
	WriteFile(path+"probs",newprobs);
	LoadProblems();
	LoadGroup();
	if(Prob->count()>=1)
	{
		Prob->setCurrentItem(0);
		LoadProb();
		LoadCode();
	}	
	QFile(path+pname+".txt").remove();
	QFile(path+pname+".cpp").remove();
	QFile(path+pname+"test").remove();
	QFile(path+"."+pname+"e").remove();
}


void admin::UList_doubleClicked()
{
	addrem* ar= new addrem(this);
	ar->exec();
}


void admin::TUpList_doubleClicked()
{
	addrem* ar= new addrem(this);
	ar->exec();
}


void admin::shour3_valueChanged()
{
    Apply->setEnabled(true);
}


void admin::sminute3_valueChanged()
{
     Apply->setEnabled(true);
}


void admin::ssec3_valueChanged()
{
     Apply->setEnabled(true);
}


void admin::SavePS_clicked()
{
	QString PSet;
	for(uint i=0;i<problems.size();i++)
	{
		PSet +="<g>\n";
		for(uint j=0;j<problems.at(i).size();j++)
		{
			QString pname = QString::number(problems.at(i).at(j)) ;
			PSet += "<pname>\n"+pname+"\n</pname>\n" ;
			PSet += "<ques>\n"+ReadFile(path+pname+".txt")+"\n</ques>\n";
			PSet += "<code>\n"+ReadFile(path+pname+".cpp")+"\n</code>\n";
			PSet += "<cases>\n"+ReadFile(path+pname+"test")+"\n</cases>\n";
			PSet += "<examples>\n"+ReadFile(path+"."+pname+"e")+"\n</examples>\n";
		}
		PSet += "</g>\n";
	}
	QString fpath = QFileDialog::getSaveFileName ( QDir::homeDirPath(),"*.pset",this,0,"Save Current Problem Set As");
	if(fpath != QString::null)
	{
		if(fpath.right(5) != ".pset")
			fpath += ".pset" ;
		WriteFile(fpath,PSet);
	}	
}


void admin::OpenPS_clicked()
{
	if(QMessageBox::warning(this, "Warning", "Are You Sure U Want to Delete current Problem Set and open a new one ??? ", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
		return;
	QString fpath = QFileDialog::getOpenFileName ( QDir::homeDirPath(), "*.pset", this,0,"Open Existing Problem Set");
	if(fpath == QString::null)
		return ;
	QString PSet = ReadFile(fpath);
	QString probs ;
	int start = 0 ;
	int i,e ;
	problems.clear();
	
	while ( ( i = PSet.find("<g>",start) ) != -1 )
	{
		i += QString("<g>").length() ;
		e = PSet.find("</g>",i)-1;
		probs += "<g>\n" ;
		QString group = PSet.mid(i,e-i+1).stripWhiteSpace();
		
		vector<int>v ;
		int i2,e2,start2=0;
		while ( ( i2 = group.find("<pname>",start2) ) != -1 )
		{
			i2 += QString("<pname>").length() ;
			e2 = group.find("</pname>",i2)-1;
			QString pn = group.mid(i2,e2-i2+1).stripWhiteSpace();
			probs += pn+"\n" ;
			v.push_back(pn.toInt());
			start2 = e2 + 1 + QString("</pname>").length();
			i2 = group.find("<ques>",start2) + QString("<ques>").length() ;
			e2 = group.find("</ques>",i2)-1;
			QString ques = group.mid(i2,e2-i2+1).stripWhiteSpace();
			start2 = e2 + 1 + QString("</ques>").length();
			WriteFile(path+pn+".txt",ques);
			i2 = group.find("<code>",start2) + QString("<code>").length() ;
			e2  = group.find("</code>",i2) - 1 ;
			QString code = group.mid(i2,e2-i2+1).stripWhiteSpace() ;
			start = e2 + 1 + QString("</code>").length();
			WriteFile(path+pn+".cpp",code);
			i2 = group.find("<cases>",start2) + QString("<cases>").length() ;
			e2 = group.find("</cases>",i2)-1;
			QString test_cases = group.mid(i2,e2-i2+1).stripWhiteSpace();
			WriteFile(path+pn+"test",test_cases);
			start2 = e2+1+QString("</cases>").length();
			i2 = group.find("<examples>",start2) + QString("<examples>").length() ;
			e2 = group.find("</examples>",i2)-1;
			QString egs = group.mid(i2,e2-i2+1).stripWhiteSpace();
			WriteFile(path+"."+pn+"e",egs);
			start2 = e2+1+QString("</examples>").length();
		}
		probs += "</g>\n";
		problems.push_back(v);
		start = e + 1 + QString("</g>").length();
	}
	WriteFile(path+"probs",probs);
	LoadProblems();
	grp->clear();
	for(uint j=0;j<problems.size();j++)
		grp->insertItem(QString::number(j+1));
	if(grp->count()>0)
		grp->setCurrentItem(0);
	LoadGroup();
	LoadProb();
	LoadCode();
}


void admin::Remove_clicked()
{
	if(grp->count()==0)
		return ;
	if(QMessageBox::warning(this, "Warning", "Are You Sure ??? ", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
		return;
	WriteFile(path+"probs","");
	
	for(uint i=0;i<problems.size();i++)
	{
		for(uint j=0;j<problems.at(i).size();j++)
		{
			QString pdel = QString::number(problems.at(i).at(j));
			QFile(path+pdel+".txt").remove();
			QFile(path+pdel+".cpp").remove();
			QFile(path+pdel+"test").remove();
			QFile(path+"."+pdel+"e").remove();
		}
	}
	Tab_selected("Problems");
}

void admin ::LoadUserCode()
{
	if(Teamcmb->count()==0 || Problemcmb->count() == 0)
		return ;
	UserCode->setText(ReadFile(QDir::homeDirPath()+"/users/"+Teamcmb->currentText()+"/"+Problemcmb->currentText()+".cpp"));
}
void admin::Teamcmb_activated()
{
	PopulateCombo(Teamcmb,Problemcmb);
	LoadUserCode();
}


void admin::Problemcmb_activated()
{
	LoadUserCode();
}


void admin::Examples_clicked()
{
	if(Prob->count() == 0)
		return ;
	WriteFile(path+".p",Prob->currentText());
	WriteFile(path+".mode","egs");
	io* i = new io();
	i->exec();
}


void admin::grp_activated()
{
	LoadGroup();
	LoadProb();
	LoadCode();	
}


void admin::Add_2_clicked()
{
	WriteFile(path+"probs",ReadFile(path+"probs")+"<g>\n</g>\n");
	LoadProblems();
	grp->clear();
	for(uint j=0;j<problems.size();j++)
		grp->insertItem(QString::number(j+1));
	grp->setCurrentItem(grp->count()-1);
	LoadGroup();
	LoadProb();
	LoadCode();
	checkBox1->setChecked(false);
	checkBox2->setChecked(false);
	ProbText->clear();
	Code->clear();
}


void admin::SavePS_2_clicked()
{
	if(grp->count()==0)
		return ;
	if(Prob->count () == 0)
		return ;
	QString prob;
	QString pname = Prob->currentText();
	prob += "<ques>\n"+ReadFile(path+pname+".txt")+"\n</ques>\n";
	prob += "<code>\n"+ReadFile(path+pname+".cpp")+"\n</code>\n";
	prob += "<cases>\n"+ReadFile(path+pname+"test")+"\n</cases>\n";
	prob += "<examples>\n"+ReadFile(path+"."+pname+"e")+"\n</examples>\n";
	QString fpath = QFileDialog::getSaveFileName ( QDir::homeDirPath(),pname+".prob",this,0,"Save Current Problem");
	if(fpath != QString::null)
	{
		if(fpath.right(5) != ".prob")
			fpath += ".prob" ;
		WriteFile(fpath,prob);
	}	
}


void admin::OpenPS_2_clicked()
{
	if(grp->count()==0)
		Add_2_clicked();
	if(Prob->count()==0)
	{
		MsgBox("","Make a problem First");
		return ;
	}
	QString fpath = QFileDialog::getOpenFileName ( QDir::homeDirPath(), "*.prob", this,0,"Open Existing Problem");
	if(fpath == QString::null)
		return ;
	QString prob = ReadFile(fpath);
	QString pn=Prob->currentText();
	int start = 0 ;
	int i,e ;
	while ( ( i = prob.find("<ques>",start) ) != -1 )
	{
		i += QString("<ques>").length() ;
		e = prob.find("</ques>",i)-1;
		QString ques = prob.mid(i,e-i+1).stripWhiteSpace();
		start = e + 1 + QString("</ques>").length();
		WriteFile(path+pn+".txt",ques);
		i = prob.find("<code>",start) + QString("<code>").length() ;
		e  = prob.find("</code>",i) - 1 ;
		QString code = prob.mid(i,e-i+1).stripWhiteSpace() ;
		start = e + 1 + QString("</code>").length();
		WriteFile(path+pn+".cpp",code);
		i = prob.find("<cases>",start) + QString("<cases>").length() ;
		e = prob.find("</cases>",i)-1;
		QString test_cases = prob.mid(i,e-i+1).stripWhiteSpace();
		WriteFile(path+pn+"test",test_cases);
		start = e+1+QString("</cases>").length();
		i = prob.find("<examples>",start) + QString("<examples>").length() ;
		e = prob.find("</examples>",i)-1;
		QString egs = prob.mid(i,e-i+1).stripWhiteSpace();
		WriteFile(path+"."+pn+"e",egs);
		start = e+1+QString("</examples>").length();
	}
	LoadProb();
	LoadCode();
}


void admin::Rem_2_clicked()
{
	if(grp->count ()==0)
		return ;
	if(QMessageBox::warning(this, "Warning", "Are You Sure ??? ", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
		return;
	int gindex =grp->currentText().toInt()-1; 
	problems.erase(problems.begin()+gindex);
	
	for(int i=0;i<Prob->count ();i++)
	{	
		QString pname = Prob->text(i);
		QFile(path+pname+".txt").remove();
		QFile(path+pname+".cpp").remove();
		QFile(path+pname+"test").remove();
		QFile(path+"."+pname+"e").remove();
	}	
	QString newprobs ;
	for(uint i=0;i<problems.size();i++)
	{
		newprobs +="<g>\n";
		for(uint j=0;j<problems.at(i).size();j++)
			newprobs += QString::number(problems.at(i).at(j))+"\n" ;
		newprobs += "</g>\n";
	}
	WriteFile(path+"probs",newprobs);
	LoadProblems();
	grp->clear();
	for(uint j=0;j<problems.size();j++)
		grp->insertItem(QString::number(j+1));
	if(grp->count()>0)
		grp->setCurrentItem(grp->count()-1);
	LoadGroup();
	if(Prob->count()>=1)
	{
		Prob->setCurrentItem(0);
		LoadProb();
		LoadCode();
	}	
}


void admin::ProbText_doubleClicked()
{
	//Code->setHidden(1- Code->isHidden());
	//textLabel1_2->setHidden(1- textLabel1_2->isHidden());
	//checkBox2->setHidden(1- checkBox2->isHidden());
	//Load2->setHidden(1-Load2->isHidden());
}

void admin::Log_doubleClicked()
{
	if(QMessageBox::warning(this, "Warning", "Are You Sure u want to clear all lists??? ", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
		return;
	QFile(path+".user_list").remove();
	QFile(path+".user_timeup").remove();
	QFile(path+"queue").remove();
	QFile(path+"log.txt").remove();
	QString command = "rm -fr ";
	command += QDir::homeDirPath();
	command += "/users" ; 
	system(command);
}

void* start(void* arg)
{
	system((char*)arg);
	compile_over=true ;
	return arg;
}
void admin :: CheckPT()
{
	if(compile_over)
	{
		timer->stop();
		QString msg ;
		if(QFile::exists(path+"a.out"))
			co->setText("Code Compiled Successfully !!!\n\n"+ReadFile(path+"op.txt"));
		else
			co->setText(ReadFile(path+"op.txt").replace(path,""));
		QFile(path+"op.txt").remove();
		compile->setText("Compile");
	}
}
void admin::compile_clicked()
{	
	co->clear();
	if(Code->text()[Code->text().length()-1]!='\n')
		Code->setText(Code->text()+"\n");
	compile_over=false;
	if(timer !=NULL)
		delete timer;
	timer = new QTimer(this);
	connect( timer, SIGNAL(timeout()),this, SLOT(CheckPT()) );	
	QString comm = "g++ -o " +path+"a.out " +path+Prob->currentText()+".cpp >& "+path+"op.txt";
	char* command = new char[comm.length()+1];
	for(uint i=0;i<comm.length();i++)
		command[i]=QChar(comm.at(i));
	command[comm.length()]='\0';
	if(pt!=NULL)
		delete pt ;
	pt = new pthread_t ;
	QFile(path+"a.out").remove();
	pthread_create(pt,NULL,start,command);
	compile->setText("Compiling ...");
	timer->start( 100, FALSE );
}

void admin :: CheckThread()
{
	if(binary_over)
	{
		timer->stop();
		Compile->setEnabled(true);	
		Teamcmb->setEnabled(true);
		Problemcmb->setEnabled(true);
	}
}
void* START(void* arg)
{
	system((char*)arg);
	binary_over=true ;
	return arg;
}
void admin::Compile_clicked()
{
	Compile->setEnabled(false);
	Teamcmb->setEnabled(false);
	Problemcmb->setEnabled(false);
	binary_over=false;
	if(timer !=NULL)
		delete timer;
	timer = new QTimer(this);
	connect( timer, SIGNAL(timeout()),this, SLOT(CheckThread()) );	
	QString binarypath = QDir::homeDirPath()+"/users/"+Teamcmb->currentText()+"/"+Problemcmb->currentText();
	QString codepath = QDir::homeDirPath()+"/users/"+Teamcmb->currentText()+"/"+Problemcmb->currentText()+".cpp" ;
	QString comm = "g++ -o " +binarypath+" "+codepath;
	char* command = new char[comm.length()+1];
	for(uint i=0;i<comm.length();i++)
		command[i]=QChar(comm.at(i));
	command[comm.length()]='\0';
	if(pt!=NULL)
		delete pt ;
	pt = new pthread_t ;
	QFile(binarypath).remove();
	pthread_create(pt,NULL,START,command);
	timer->start( 100, FALSE );
}

