/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <qfile.h>
#include <qtextstream.h>
#include <qdir.h>
#include <qmessagebox.h>
#include <qtimer.h>
#include "decl.h"

#define DELAY 	100

using namespace std ;

QString path_score;

void score::init()
{
	ok->setAccel(Qt::Key_Return);
	GetPath(path_score);
	ok->setText("Please Wait ...");
	QTimer::singleShot( DELAY, this, SLOT(LoadSubmissions()) );	
}

void score :: LoadSubmissions()
{
	QFile(path_score+".allscores").remove();
	bool flagg = false;
	while(system(path_score +"./client 6")!=0)
	{	
		
		int opt = QMessageBox::critical( this, "Error","Error Connecting To The Server","Retry","Cancel", 0, 0, 1 ) ;
		switch(opt)
		{
			case 0: break ;
			case 1:  flagg =true ;goto end;
		}			
		QFile(path_score+".allscores").remove();
	}
	txtscore->setText(ReadFile(path_score+".allscores"));
	ok->setText("OK");
	end:
	if(flagg)
		this->close();
}

void score::ok_clicked()
{
	this->close();
}
