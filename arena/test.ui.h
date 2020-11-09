#include <qdir.h>
#include <qmessagebox.h>
#include <iostream>
#include <sys/resource.h>
#include<sys/types.h>
#include <qdatetime.h>
#include <qevent.h>
#include <qstring.h>
#include <vector>
#include <qapplication.h>
#include <qprocess.h>
#include <pthread.h>
#include <qtimer.h>
#include <signal.h>
#include "decl.h"

#define HARDLIMIT 8000

using namespace std;

pthread_t* pt =NULL;
QTimer* timer = NULL ;

bool exitflag =false;
QString path_test;
int timelimit ;
vector< pair<QString,QString> > io ;
int eno ;
int exit_status ;
int START_TIME;
FILE* fi ;
FILE* fo;

QProcess* p = NULL ;

QString TimeLimitStartTag =	"<tl>";
QString TimeLimitEndTag =	"</tl>";
QString InputStartTag = 	"<i>";
QString InputEndTag =	"</i>";
QString OutputStartTag = "<o>";
QString OutputEndTag = "</o>";

void test::closeEvent(QCloseEvent* ce)
{
	if(pt != NULL)
		pthread_cancel(*pt);
	ce->accept();
}	

void test::LoadExamples()
{
	int i,e;
	QString egs = ReadFile(path_test + ".egs") ;
	i = egs.find(TimeLimitStartTag,0) + TimeLimitStartTag.length();
	e =  egs.find(TimeLimitEndTag,i)-1 ;
	timelimit=egs.mid(i,e-i+1).simplifyWhiteSpace().toInt();
	lbltl->setText("TimeLimit : "+QString::number(timelimit)+" msecs");
	int start = 0 ;
	io.clear();
	pair <QString,QString> p ;
	while ( ( i = egs.find(InputStartTag,start) ) != -1 )
	{
		i +=InputStartTag.length() ;
		e = egs.find(InputEndTag,i)-1;
		p.first= egs.mid(i,e-i+1).stripWhiteSpace();
		start =e+1+InputEndTag.length() ;
		i = egs.find(OutputStartTag,start) + OutputStartTag.length() ;
		e = egs.find(OutputEndTag,e)-1;
		p.second = egs.mid(i,e-i+1).stripWhiteSpace();
		io.push_back(p);
		start = e+1+ OutputEndTag.length();
	}
	cmb->clear();
	for(uint j=0;j<io.size();j++)
		cmb-> insertItem ("Example "+QString::number(j+1));
	if(cmb->count()>0)
		cmb->setCurrentItem(0);
	LoadExample();
}

void test::init ()
{
	GetPath(path_test);
	LoadExamples();
	lblwait->clear();
}

void test::close_clicked()
{
	this->close();
}

void test::LoadExample()
{
	if ( rbcustom->isChecked())
	{
		cmb->setEnabled(false);
		txtip->clear();
		txtip->setReadOnly(false);
		msg->clear();
		txtop->clear();
		txtip->setFocus();
	}
	else
	{
		cmb->setEnabled(true);
		int s = cmb->currentText().find(" ",0) ;
		int e = cmb->currentText().length()-1;
		eno = cmb->currentText().mid(s+1,e-s).toInt();
		txtip->setText(io.at(eno-1).first);
		txtip->setReadOnly(true);
		txtop->clear();
		msg->clear();
	}	
}

void test::cmb_activated()
{
	LoadExample();
}

void test::txtip_textChanged()
{
	txtop->clear();
}

void test::Exit()
{
	if(!QFile::exists(path_test+".egs"))
		MsgBoxC("Error","Error Connecting to the Server");
	else
		LoadExamples();
	refresh->setText("Refresh");
}
void test::refresh_clicked()
{
	if(refresh->text()!="Refresh")
		return ;
	if(QMessageBox::warning(this, "Warning", "Are You Sure U want to refresh the examples", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
		return ;
	refresh->setText("Refreshing ...");
	if(p!=NULL)
		delete p;
	p = new QProcess(this);
	p->setWorkingDirectory(path_test);
	p->addArgument ( "./client");
	p->addArgument ( "11");
	p->addArgument("1");
	connect( p, SIGNAL( processExited()),this, SLOT(Exit()) );
	QFile(path_test+".egs").remove();
	p->start();
}
void* start(void* arg)
{
	exit_status = system((char*)arg);
	exitflag =true ;
	pthread_exit(arg);
}

void test::btntest_clicked()
{
	txtop->clear();
	msg->clear();
	btntest->setEnabled(false);
	//Close->setEnabled(false);
	lblwait->setText("Please Wait");
	refresh->setEnabled(false);;
	cmb->setEnabled(false);
	msg->setFocus();
	if(txtip->text().simplifyWhiteSpace()=="")
		WriteFile(path_test+"input.txt","0");
	else
		WriteFile(path_test+"input.txt",txtip->text());
	WriteFile(path_test+"output.txt","");
	fi  = freopen(path_test+"input.txt","r",stdin);
	fo = freopen(path_test+"output.txt","w",stdout);
	exitflag =false;
	if(timer !=NULL)
		delete timer;
	timer = new QTimer(this);
	if(pt !=NULL)
		delete pt;
	pt = new pthread_t;
	connect( timer, SIGNAL(timeout()),this, SLOT(CheckPThread()) );	
		
	QString path = path_test+"./test";
	char* command = new char[path.length()+1];
	for(uint i=0;i<path.length();i++)
		command[i]=QChar(path.at(i));
	command[path.length()]='\0';
	pthread_attr_t pta;
	pthread_attr_init(&pta);
	pthread_attr_setdetachstate(&pta,PTHREAD_CREATE_DETACHED);
	if(pthread_create (pt,&pta,start,command)!=0)
	{
		pthread_attr_destroy(&pta);
		MsgBoxC("Error","Error Executing Code");
		return ;
	}
	pthread_attr_destroy(&pta);	
	START_TIME = ToMSecs(QTime::currentTime());
	timer->start( 50, FALSE );
}
void test::CheckPThread()
{
	
	if(lblwait->text()=="Please Wait")
		lblwait->setText("Please Wait .");
	else if (lblwait->text()=="Please Wait .")
		lblwait->setText("Please Wait ..");
	else if (lblwait->text()=="Please Wait ..")
		lblwait->setText("Please Wait ...");
	else if (lblwait->text()=="Please Wait ...")
		lblwait->setText("Please Wait ....");
	else if (lblwait->text()=="Please Wait ....")
		lblwait->setText("Please Wait .....");
	else if (lblwait->text()=="Please Wait .....")
		lblwait->setText("Please Wait ......");
	else if (lblwait->text()=="Please Wait ......")
		lblwait->setText("Please Wait .......");
	else if (lblwait->text()=="Please Wait .......")
		lblwait->setText("Please Wait ........");
	else if (lblwait->text()=="Please Wait ........")
		lblwait->setText("Please Wait .........");
	else if (lblwait->text()=="Please Wait .........")
		lblwait->setText("Please Wait ..........");
	else if (lblwait->text()=="Please Wait ..........")
		lblwait->setText("Please Wait");
	
	if( exitflag )
	{
		timer->stop();
		QString pop = ReadFile(path_test+"output.txt") ;
		txtop->setText(pop.stripWhiteSpace());
		if(exit_status != 0)
		{
			msg->append("<font color=\"darkRed\"> Program Exited Abnormally With Error : ( "+QString(strsignal(exit_status))+" )</font>");
			goto end;
		}	
		if(rbcustom->isChecked())
		{
			msg->append("Check the correctness of the output yourself");
			goto end;
		}	
		if(pop.stripWhiteSpace()!=io.at(eno-1).second)
		{
			msg->append("<font color=\"darkRed\">Expected Output : <br><br>"+io.at(eno-1).second+"<br></font>");
			msg->append("<font color=\"darkRed\">UnSuccessfull<br></font>");
		}	
		else
			msg->append("<font color=\"darkGreen\">Correct Output<br></font>");						
		
		end:
	
		fclose(fi);
		fclose(fo);
		QFile(path_test+"input.txt").remove();
		QFile(path_test+"output.txt").remove();
		btntest->setEnabled(true);
		Close->setEnabled(true);
		lblwait->clear();
		refresh->setEnabled(true);;
		if(rbegs->isChecked())
			cmb->setEnabled(true);
		txtip->setReadOnly(false);
		return ;
	}	
	if( ToMSecs(QTime::currentTime())-START_TIME  >= HARDLIMIT)
	{
		pthread_cancel(*pt);
		msg->setText("<font color=\"darkRed\">UnSuccessfull<br><br>Program Terminated because it didn't respond within "+QString::number(HARDLIMIT)+" msecs" );
		timer->stop();
		fclose(fi);
		fclose(fo);
		QFile(path_test+"input.txt").remove();
		QFile(path_test+"output.txt").remove();
		btntest->setEnabled(true);
		Close->setEnabled(true);
		lblwait->clear();
		refresh->setEnabled(true);;
		if(rbegs->isChecked())
			cmb->setEnabled(true);
		txtip->setReadOnly(false);
	}
}
void test::rbcustom_clicked()
{
	LoadExample();
}
void test::rbegs_clicked()
{
	LoadExample();
}
