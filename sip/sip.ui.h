#include <qdir.h>
#include <qfile.h>
#include <qstring.h>
#include <qmessagebox.h>
#include <qevent.h>

using namespace std ;
QString path_sip;

QString sip :: ReadFile ( QString fname ) 
{
	QString data ;
	QFile f (fname) ;
	
	if ( f.open( IO_ReadOnly) )
	{
		data="";
	    QTextStream s (&f);
		while ( s.atEnd( ) == false )
		{
			 data += s.readLine( ) ;
			 if ( s.atEnd() == false )
			 	 data += "\n";
		}
		f.close();
	}
	else
		data="-1";
	return data ;
}

bool sip :: WriteFile ( QString fname , QString data ) 
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
void sip :: GetPath()
{
	path_sip = QDir::homeDirPath()+"/.files/";
}
void sip :: init ()
{
	GetPath();
	QString temp =  ReadFile(path_sip+".serverip");
	if(temp == "-1")
		WriteFile(path_sip+".serverip","127.0.0.1");
	serverip->setText(ReadFile(path_sip+".serverip"));
}

void sip::closeEvent(QCloseEvent* ce)
{
	if(serverip->text().length() < 9 || serverip->text().contains(".")!=3)
	{
		MsgBox("Invalid IP","Please enter a valid IP");
		return ;
	}	
	if(QMessageBox::warning(this, "Warning", "Are You Sure??", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
		return ;
	WriteFile(path_sip+".serverip",serverip->text());
	ce->accept();
}	
void sip::MsgBox(QString title, QString msg)
{
	QMessageBox* m = new QMessageBox(title,msg,QMessageBox::Critical,QMessageBox::Ok,QMessageBox::NoButton, QMessageBox::NoButton);
	m->exec();
}
void sip::ok_clicked()
{
	this->close();
}
