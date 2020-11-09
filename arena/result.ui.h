#include <qdir.h>
#include <qstring.h>
#include <qmessagebox.h>
#include <qfile.h>
#include <qtimer.h>
#include "decl.h"

#define DELAY 	100

QString path_result;

void result::init()
{
	ok->setAccel(Qt::Key_Return);
	GetPath(path_result);
	ok->setText("Please Wait ...");
	QTimer::singleShot( DELAY, this, SLOT(LoadResult()) );	
}

void result :: LoadResult()
{
	QFile(path_result+".result").remove();
	QFile(path_result+".testerop").remove();
	bool flag = false;	
	
	system(path_result+"./client 10 " + ReadFile(path_result+".team"));
	while(!QFile::exists(path_result+".result") || !QFile::exists(path_result+".testerop"))
	{
		int opt = QMessageBox::critical( this, "Error","Error Contacting Server","Retry","Cancel", 0, 0, 1 ) ;
		switch(opt)
		{
			case 0: break ;
		       case 1: flag=true;
				   goto end ;	   
		}
		QFile(path_result+".result").remove();
		QFile(path_result+".testerop").remove();
		system(path_result+"./client 10 " + ReadFile(path_result+".team"));
	}
	TesterOP->setText(ReadFile(path_result+".testerop")+"\n");
	Result->setText(ReadFile(path_result+".result"));
	QFile(path_result+".result").remove();
	QFile(path_result+".testerop").remove();
	ok->setText("OK");
	end:
	if(flag)
		this->close();
}

void result::ok_clicked()
{
	this->close();
}
