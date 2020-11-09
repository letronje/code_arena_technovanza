/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <qdir.h>
#include <qfile.h>
#include <qstring.h>
#include <unistd.h>
#include <qmessagebox.h>

using namespace std ;

// global variables
int AutoTest ;
int TimeWarn ;
int Warn ;
int WarningAsError;

QString path_opt;

void options::SaveOptions()
{
	QString opt=QString::number(AutoTest);
	opt += "\n" + QString::number(TimeWarn);
	opt += "\n" + QString::number(Warn);
	opt += "\n" + QString::number(WarningAsError);
	while(!WriteFile(path_opt+".options",opt))
	{	
		int opt = QMessageBox::critical( this, "Error","Error Saving Options","Retry","Cancel", 0, 0, 1 ) ;
		switch(opt)
		{
			case 0: break ;
			case 1: return ;
		}		
	}
	WriteFile(path_opt+".serverip",serverip->text());
}
void options::closeEvent(QCloseEvent* ce)
{
	SaveOptions();
	ce->accept();
}

QString options :: ReadFile ( QString fname ) 
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
bool options :: LoadOptions ( )
{
	QFile f ( path_opt+".options" ) ;
	if ( f.open(IO_ReadOnly) )
	{
		QTextStream s ( &f ) ;
		s >> AutoTest ;
		s >> TimeWarn ;
		s >> Warn ;
		s >> WarningAsError ;
		f.close();
	}
	else
		return false;
	
	chk1->setChecked(bool(AutoTest));
	
	switch(TimeWarn)
	{
		case 0 :
		{      
			r1->setChecked(true);
			break;
		}
		case 1:
		{      
			r2->setChecked(true);
			break;
		}
		case 2:
		{      
			r3->setChecked(true);
		}
	}
	
	chk2->setChecked(bool(Warn));
	chk3->setEnabled(bool(Warn));
	if(chk3->isEnabled())
		chk3->setChecked(bool(WarningAsError));
	serverip->setText(ReadFile(path_opt+".serverip"));
	return true ;
}
void options :: GetPath()
{
	path_opt = QDir::homeDirPath()+"/.files/";
}
void options::MsgBox(QString title, QString msg)
{
	QMessageBox* m = new QMessageBox(title,msg,QMessageBox::Information,QMessageBox::Ok,QMessageBox::NoButton, QMessageBox::NoButton);
	m->exec();
}
void options::init()
{
	Ok->setAccel(Qt::Key_Return);
	GetPath();
	LoadOptions();
}

bool options :: WriteFile ( QString fname , QString data ) 
{
	QFile f ( fname) ;
	
	if ( f.open( IO_WriteOnly) )
	{
		QTextStream s ( & f);
		s<<data;
		f.close();
		return true ;
	}
	else
		return false;
}

void options::Ok_clicked()
{
	this->close();
}

void options::chk1_toggled( bool state )
{
	AutoTest = state;
}

void options::chk2_toggled( bool state )
{
	Warn = state;
	chk3->setEnabled(state) ;
	if(!chk3->isEnabled())
		chk3->setChecked(false);
}

void options::r1_toggled( bool state )
{
	if ( state == true )
		TimeWarn = 0 ;
}

void options::r2_toggled( bool state )
{
	if ( state == true )
		TimeWarn = 1;
}

void options::r3_toggled( bool state )
{
	if ( state == true )
		TimeWarn = 2 ;
}

void options::chk3_toggled( bool state )
{
	WarningAsError =state ;
}
