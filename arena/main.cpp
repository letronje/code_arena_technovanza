#include <qapplication.h>
#include <qstring.h>
#include <qmessagebox.h>
#include <qfile.h>
#include <unistd.h>
#include <qtextstream.h>
#include <qdir.h>

#include "common.h"
#include "welcome.h"

void MsgBox(QString title, QString msg)
{
	QMessageBox* m = new QMessageBox(title,msg,QMessageBox::Critical,QMessageBox::Ok,QMessageBox::NoButton, QMessageBox::NoButton);
	m->exec();
}
QString GetPath()
{
	return QDir::homeDirPath ()+"/.files/";	
}

bool IsDuplicateInstance()
{
	bool result=false;
	QString path = GetPath();
	system("ps -A >& " + path+".processes");
	QString temp = ReadFile(path+".processes") ;
	int instances ;
	if ( (instances=temp.contains("arena")) > 1 )
	{
		if(instances > 2)
			MsgBox("",QString::number(instances-1)+" instances are already running !!!");
		else
			MsgBox("","An instance is already running !!!");
		result=true;
	}
	QFile(path+".processes").remove();
	return result;
}

int main( int argc, char ** argv )
{
	QApplication a( argc, argv );
	if ( IsDuplicateInstance())
		return 0;
	welcome w;
	w.show();
	a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
	return a.exec();
}
