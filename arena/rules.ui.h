
#include <qfile.h>
#include <qstring.h>
#include <qtextstream.h>
#include <qmessagebox.h>
#include <qdir.h>
#include <qtimer.h>
#include "decl.h"

#define DELAY 	100

QString path_rules;
//QTimer* timer = NULL ;

//calculate path_rules were hidden files are saved
void rules::init()
{
	ok->setAccel(Qt::Key_Return);
	GetPath(path_rules);
	ok->setText("Please Wait ...");
	QTimer::singleShot( DELAY, this, SLOT(LoadRules()) );	
}
void rules :: LoadRules()
{
	QFile(path_rules+".rules").remove();
	bool flag = false;
	while(system(path_rules +"./client 7")!=0)
	{	
		int opt = QMessageBox::critical( this, "Error","Error Connecting to the Server","Retry","Cancel", 0, 0, 1 ) ;
		switch(opt)
		{
			case 0: break ;
			case 1:  flag =true ;goto end;
		}			
		QFile(path_rules+".rules").remove();
	}
	txtrules->setText(ReadFile(path_rules+".rules"));
	ok->setText("OK");
	end:
	if(flag)
	{
		QFile(path_rules+".rules").remove();
		this->close();
	}	
}
void rules::ok_clicked()
{
	QFile(path_rules+".rules").remove();
	this->close();
}	
