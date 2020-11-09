#include <qdir.h>
#include <qfile.h>
#include <qstring.h>
#include <unistd.h>
#include <qmessagebox.h>
#include "decl.h" 

using namespace std ;

// global variables
int AutoTest ;
int TimeWarn ;
int Warn ;
int WarningAsError;
QString theme ;
QString path_opt;

void options::SaveOptions()
{
	QString opt=QString::number(AutoTest);
	opt += "\n" + QString::number(TimeWarn);
	opt += "\n" + QString::number(Warn);
	opt += "\n" + QString::number(WarningAsError);
	opt += "\n"+ theme ;
	
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
		s>>theme ;
		f.close();
	}
	else
		return false;
	for( int i=0;i<cmbt->count();i++)
	{
		if(cmbt->text(i)==theme)
		{
			cmbt->setCurrentItem(i);
			break ;
		}	
	}
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


void options::init()
{
	Ok->setAccel(Qt::Key_Return);
	GetPath(path_opt);
	LoadOptions();
}

void options::Ok_clicked()
{
	SaveOptions();
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


void options::cmbt_activated( int x)
{
	theme=cmbt->text(x);
}
