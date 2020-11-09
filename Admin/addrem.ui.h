#include <qstring.h>
#include <qdir.h>
#include <qinputdialog.h>
#include <qmessagebox.h>

using namespace std ;

QString path_addrem;
	
void addrem :: GetPath()
{
	path_addrem = QDir::homeDirPath()+"/.files/";
}
void addrem::LoadUList()
{
	ulist->clear();
	ulist->insertStringList(QStringList::split("\n",ReadFile(path_addrem+".user_list")));
	if(ulist->count () > 0)
		ulist->setCurrentItem(0);
}
void addrem::init()
{
	GetPath();
	LoadUList();
}
QString addrem :: ReadFile ( QString fname ) 
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
void addrem::MsgBox(QString title , QString msg)
{
	QMessageBox* m = new QMessageBox(title,msg,QMessageBox::Critical,QMessageBox::Ok,QMessageBox::NoButton, QMessageBox::NoButton);
	m->exec();
}
bool addrem :: AppendFile(QString fname,QString s)
{
	QFile f(fname);
	if(f.open(IO_WriteOnly | IO_Append))
	{
		QTextStream stream (&f);
		stream << s;
		f.close();
		return true ;
	}
	else
		return false;
}
bool addrem :: WriteFile ( QString fname , QString data ) 
{
	QFile f ( fname) ;
	
	if ( f.open( IO_WriteOnly) )
	{
		QTextStream s ( & f);
		s<<data;
		f.flush();
		f.close();
		return true ;
	}
	else
		return false;
}
void addrem::Add_clicked()
{
	if(QMessageBox::warning(this, "Warning", "Are You Sure??", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
			return;
	QString new_user;
	bool ok;
	
	again:
	
	ok=true;
	new_user =QInputDialog::getText ( "Add Team","Enter Team name : ", QLineEdit::Normal,"Contestant",&ok,this);
	if(!ok)
		return ;
	for(int i=0;i<ulist->count();i++)
	{
		if(ulist->text(i)==new_user.simplifyWhiteSpace())
		{
			MsgBox("Error","Team Already Exists in List ");
			return ;	   
		}	
	}
	if(new_user.length() < 3 || new_user.length() > 12 )
	{
		MsgBox("Error","Team Name Length Must Be Between 3 and 12");
		goto again;
	}	
	if(grp->currentText()=="Still in Contest")
	{
		AppendFile(path_addrem+".user_list",new_user+"\n");
		LoadUList();
	}	
	else
	{
		AppendFile(path_addrem+".user_timeup",new_user+"\n");
		LoadTUpList();
	}	
}

void addrem::Rem_clicked()
{
	if(QMessageBox::warning(this, "Warning", "Are You Sure??", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
			return;
	if(grp->currentText()=="Still in Contest")
	{
		WriteFile(path_addrem+".user_list","");
		for(int i=0;i<ulist->count();i++)
		{
			if(ulist->text(i) != ulist->currentText())
				AppendFile(path_addrem+".user_list",ulist->text(i)+"\n");
		}
		LoadUList();
	}
	else
	{
		WriteFile(path_addrem+".user_timeup","");
		for(int i=0;i<ulist->count();i++)
		{
			if(ulist->text(i) != ulist->currentText())
				AppendFile(path_addrem+".user_timeup",ulist->text(i)+"\n");
		}
		LoadTUpList();
	}	
}
void addrem :: LoadTUpList()
{
	ulist->clear();
	ulist->insertStringList(QStringList::split("\n",ReadFile(path_addrem+".user_timeup")));
	if(ulist->count () > 0)
		ulist->setCurrentItem(0);
	
}
void addrem::grp_activated( int index_sel )
{
	if(index_sel == 0)
		LoadUList();
	if(index_sel==1)
		LoadTUpList();
}

void addrem::Close_clicked()
{
	this->close();
}
