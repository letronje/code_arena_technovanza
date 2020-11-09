/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/
#include <qmessagebox.h>
#include <qstring.h>
#include <unistd.h>
#include <qfile.h>
#include <qdir.h>
#include "decl.h"

using namespace std;

QString path_reg ;

void reg::init()
{
	Register->setAccel(Qt::Key_Return);
	GetPath(path_reg);
}
//writes data to file fname

void reg::Register_clicked()
{
	Register->setEnabled(false);
	QString teamname=tname->text();
	QString col=cname->text();
	QString course=ccmb->currentText();
	if(teamname.length() > 12 || teamname.length() < 3 )
	{
		MsgBoxC("Error","Team Name Length must be between 3 and 12");
		Register->setEnabled(true);
		return ;
	}
	if( cname->text().simplifyWhiteSpace().length() < 2)
	{
		MsgBoxC("Error","Enter a Valid College Name");
		Register->setEnabled(true);
		return ;
	}	
	QFile(path_reg + ".reg").remove();
	system(path_reg+"./client " + "1 " + teamname+" "+col +" "+course);//Reg
	while(!QFile::exists(path_reg+".reg"))
	{	
		int opt = QMessageBox::critical( this, "Error","Error Contacting Server :(","Retry","Cancel", 0, 0, 1 ) ;
		switch(opt)
		{
			case 0: break ;
			case 1: Register->setEnabled(true); return ;
		}
		QFile(path_reg + ".reg").remove();
		system(path_reg+"./client " + "1 " + teamname+" "+col +" "+course);
	}
	if(ReadFile(path_reg+".reg")=="0")
	{
		MsgBoxC("Sorry","A User Has Already Registered With That Name");
		tname->setText("");
		Register->setEnabled(true);
	}
	else if(ReadFile(path_reg+".reg")=="1")
	{
		MsgBoxI("Success","Registered Successfully");
		WriteFile(path_reg+".areg","1");
		WriteFile(path_reg+".team",teamname);
		system(path_reg+"./client "+"2 "+ teamname);//GetNoProbs
		while(!QFile::exists(path_reg+".probs"))
		{
			int opt = QMessageBox::critical( this, "Error","Error Loading Time","Retry","Quit", 0, 0, 1 ) ;
			switch(opt)
			{
				case 0: break ;
				case 1: exit(1);
			}
			system(path_reg+"./client "+"2 "+ teamname);
		}	
		Register->setEnabled(true);
		this->close();
	}
		
	Register->setEnabled(true);
	QFile(path_reg + ".reg").remove();
}
void reg::closeEvent(QCloseEvent* ce)
{
    ce->accept();
}

void reg::tname_textChanged()
{
	tname->setText(tname->text().replace(" ","_")); 
}


void reg::cname_textChanged()
{
	cname->setText(cname->text().replace(" ","_")); 
}
