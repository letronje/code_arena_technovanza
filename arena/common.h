QString ReadFile ( const QString& fname ) 
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
bool WriteFile ( const QString & fname, const QString & data) 
{
	QFile f ( fname) ;
	
	if ( f.open( IO_WriteOnly) )
	{
		QTextStream s (&f);
		s<<data;f.flush();
		f.close();
		return true;
	}
	else
		return false;
}
void MsgBoxI(const QString&  title, const QString& msg)
{
	QMessageBox* m = new QMessageBox(title,msg,QMessageBox::Information ,QMessageBox::Ok,QMessageBox::NoButton, QMessageBox::NoButton);
	m->exec();
}
void MsgBoxW(const QString&  title, const QString& msg)
{
	QMessageBox* m = new QMessageBox(title,msg,QMessageBox::Warning,QMessageBox::Ok,QMessageBox::NoButton, QMessageBox::NoButton);
	m->exec();
}
void MsgBoxC(const QString& title , const QString& msg)
{
	QMessageBox* m = new QMessageBox(title,msg,QMessageBox::Critical,QMessageBox::Ok,QMessageBox::NoButton, QMessageBox::NoButton);
	m->exec();
}
void GetPath(QString& s)
{
	s = QDir::homeDirPath()+"/.files/";
}
int ToSeconds(const QTime& t )
{
	return (( t.hour()*3600 + t.minute()*60 + t.second() ));
}
int ToMSecs(const QTime& t )
{
	return (ToSeconds(t)*1000);
}

