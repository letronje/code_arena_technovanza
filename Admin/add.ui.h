#include <qstring.h>
#include <qmessagebox.h>
#include <qinputdialog.h> 
#include <qfiledialog.h> 
#include <qdir.h>

#include "io.h"

using namespace std ;

QString path_add;

void add::GetPath()
{
	path_add = QDir::homeDirPath()+"/.files/";
}
void add::MsgBox(QString title , QString msg)
{
	QMessageBox* m = new QMessageBox(title,msg,QMessageBox::Critical,QMessageBox::Ok,QMessageBox::NoButton, QMessageBox::NoButton);
	m->exec();
}

void add::Load1_clicked()
{
	if(QMessageBox::warning(this, "Warning", "Are You Sure?? ", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
			return;
	QString fname = QFileDialog::getOpenFileName (QDir::homeDirPath() , "*.txt *.doc *.cpp *.c *.rtf ",this,0,"Open Problem From A File");
	if(fname!=QString::null)
		ProbText->setText(ReadFile(fname));
}

QString add :: ReadFile ( QString fname ) 
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

void add::Load2_clicked()
{
	if(QMessageBox::warning(this, "Warning", "Are You Sure?? ", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
			return;
	QString fname = QFileDialog::getOpenFileName (QDir::homeDirPath() , "*.txt *.doc *.cpp *.c *.rtf ",this,0,"Open Problem From A File");
	if(fname!=QString::null)
		Code->setText(ReadFile(fname));
}

void add::init()
{
	GetPath();
}
void add::Done_clicked()
{
	if(QMessageBox::warning(this, "Warning", "Are You Sure?? ", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
			return;
	io* i = new io(this);
	i->exec();
	/*bool ok;
	QString pname ;
	
	again:
	
	pname = QString::number(QInputDialog::getInteger ( "Points", "Enter No Of Points : ",1000, 50, 9999, 25, &ok, this ));
	MsgBox("",pname);
	if(!ok )
		return ;
	if(QFile::exists(path_add+pname+".txt"))
	{
		MsgBox("Error","Problem Already Exists");
		goto again;
	}	
	int no_cases = QInputDialog::getInteger ( "Cases", "Enter No Of Test Cases : ",10,3,25,1,&ok,this);
	if(!ok)
		return ;
	QString test;
	for ( int i=0;i<no_cases;i++)
	{
			io* i = new io(this);
			i->exec();
			delete i;
			test += ReadFile(path_add+".iop")+"\n";
	}
	MsgBox("",test);
	*/
}
