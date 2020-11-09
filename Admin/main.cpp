#include <qapplication.h>
#include <qfile.h>
#include <qmessagebox.h>
#include <qdir.h>
#include "admin.h"

QString GetPath()
{
    return QDir::homeDirPath()+"/.files/";
}
QString ReadFile ( QString fname ) 
{
	QString data="-1" ;
	QFile f (fname) ;
	
	if ( f.open( IO_ReadOnly) )
	{
		data="";
	    QTextStream s (&f);
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

void MsgBox(QString title, QString msg)
{
	QMessageBox* m = new QMessageBox(title,msg,QMessageBox::Critical,QMessageBox::Ok,QMessageBox::NoButton, QMessageBox::NoButton);
	m->exec();
}
bool IsDuplicateInstance()
{
	bool result=false;
	QString path = GetPath();
	system("ps -A >& " + path+".processes");
	QString temp = ReadFile(path+".processes") ;
	int instances ;
	if ( (instances=temp.contains("admin")) > 1 )
	{
		if(instances > 2)
			MsgBox("",QString::number(instances-1)+" instances are already running !!!");
		else
			MsgBox("","An instance is already running !!!");
		result=true;
	}
	system("rm -f "+path+".processes");
	return result;
}

int main( int argc, char ** argv )
{
	QApplication a( argc, argv );
	if ( IsDuplicateInstance())
		return 0;
	admin w;
	w.show();
	a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
	return a.exec();
}
