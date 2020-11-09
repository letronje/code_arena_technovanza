#include <qmessagebox.h>
#include <qstring.h>
#include <qdatetime.h>
#include <qtextstream.h>
#include <qfile.h>
#include <qdir.h>
#include <vector>
#include <qinputdialog.h>
#include <qmessagebox.h>
#include <qprocess.h>
#include <qevent.h>

using namespace std ;

int mode =0;

QString path_io;
QString pname ;

vector <QString> ips;
vector <QString> ops;
vector <QString> pers;

ulong t_limit;
int case_index=0;
int egs_index = 0;

int no_cases;
int no_egs ;

bool compiled =false;

QString IST  = "<i>" ;
QString IET  = "</i>";
QString OST = "<o>" ;
QString OET = "</o>";
QString PST = "<p>";
QString PET = "</p>";
QString TST = "<tl>";
QString TET = "</tl>";

QProcess* proc = NULL ;
int st;

void io::MsgBox(QString title , QString msg)
{
	QMessageBox* m = new QMessageBox(title,msg,QMessageBox::Critical,QMessageBox::Ok,QMessageBox::NoButton, QMessageBox::NoButton);
	m->exec();
}
void io::closeEvent(QCloseEvent* ce)
{
	ce->accept();
}
bool io :: WriteFile ( QString fname , QString data ) 
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
void io::GetPath()
{
	path_io = QDir::homeDirPath()+"/.files/";
}
QString io :: ReadFile ( QString fname ) 
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
QString io::DefaultCase()
{
	QString temp = TST+"\n100\n"+TET+"\n" ;
	temp += IST+"\n"+""+"\n"+IET+"\n";
	temp += OST+"\n"+""+"\n"+OET+"\n";
	temp += PST+"\n"+"100"+"\n"+PET+"\n";
	return temp ;
}
QString io::DefaultExample()
{
	QString temp = TST+"\n100\n"+TET+"\n" ;
	temp += IST+"\n"+""+"\n"+IET+"\n";
	temp += OST+"\n"+""+"\n"+OET+"\n";
	return temp ;
}
void io :: LoadIOP()
{
	if(mode == 0)
	{
		pname = ReadFile(path_io+".p");
		if(!QFile::exists(path_io + pname+"test"))
			WriteFile(path_io + pname+"test",DefaultCase());
		QString cases = ReadFile(path_io + pname+"test") ;
		int tlsi = cases.find(TST,0) ;
		tlsi += TST.length();
		int tlei =  cases.find(TET,tlsi)-1 ;
		QString tlimit=cases.mid(tlsi,tlei-tlsi+1);
		tlsb->setValue(tlimit.simplifyWhiteSpace().toInt());
		int start = 0 ;
		int input_start_index ;
		ips.clear();
		ops.clear();
		pers.clear();
		case_index=0;
		no_cases=0;
		while ( ( input_start_index = cases.find(IST,start) ) != -1 )
		{
			input_start_index += IST.length() ;
			int input_end_index = cases.find(IET,input_start_index)-1;
			QString i = cases.mid(input_start_index,input_end_index-input_start_index+1);
			ips.push_back( i.stripWhiteSpace());
			start = input_end_index + 1 + IET.length();
			int output_start_index = cases.find(OST,start) + OST.length() ;
			int output_end_index = cases.find(OET,output_start_index)-1;
			QString o = cases.mid(output_start_index,output_end_index-output_start_index+1);
			ops.push_back(o.stripWhiteSpace());
			start = output_end_index + 1 + OET.length();
			int percentage_start_index = cases.find(PST,start) + PST.length() ;
			int percentage_end_index  = cases.find(PET,percentage_start_index) - 1 ;
			QString per = cases.mid(percentage_start_index,percentage_end_index-percentage_start_index+1) ;
			pers.push_back(per.simplifyWhiteSpace());
			start = percentage_end_index + 1 + PET.length();
		}	
		no_cases = ips.size();
		if(no_cases==0)
		{
			ips.push_back("");
			ops.push_back("");
			pers.push_back("100");
			no_cases=1;
		}	
		if(no_cases>1)
		{
			Next->setEnabled(true);
			Sub->setEnabled(true);
		}
		if(no_cases >=1)
		{	
			i->setText(ips.at(0));
			o->setText(ops.at(0));
			p->setValue(pers.at(0).toInt());
		}	
	}	
	else if(mode ==1)
	{
		pname = ReadFile(path_io+".p");
		if(!QFile::exists(path_io + "."+pname+"e"))
			WriteFile(path_io + "."+pname+"e",DefaultExample());
		QString egs = ReadFile(path_io + "."+pname+"e") ;
		int tlsi = egs.find(TST,0) ;
		tlsi += TST.length();
		int tlei =  egs.find(TET,tlsi)-1 ;
		QString tlimit=egs.mid(tlsi,tlei-tlsi+1);
		tlsb->setValue(tlimit.simplifyWhiteSpace().toInt());
		int start = 0 ;
		int input_start_index ;
		ips.clear();
		ops.clear();
		egs_index=0;
		no_egs=0;
		while ( ( input_start_index = egs.find(IST,start) ) != -1 )
		{
			input_start_index += IST.length() ;
			int input_end_index = egs.find(IET,input_start_index)-1;
			QString i = egs.mid(input_start_index,input_end_index-input_start_index+1);
			ips.push_back( i.stripWhiteSpace());
			start = input_end_index + 1 + IET.length();
			int output_start_index = egs.find(OST,start) + OST.length() ;
			int output_end_index = egs.find(OET,output_start_index)-1;
			QString o = egs.mid(output_start_index,output_end_index-output_start_index+1);
			ops.push_back(o.stripWhiteSpace());
			start = output_end_index + 1 + OET.length();
		}	
		no_egs = ips.size();
		if(no_egs==0)
		{
			ips.push_back("");
			ops.push_back("");
			no_egs=1;
		}	
		if(no_egs>1)
		{
			Next->setEnabled(true);
			Sub->setEnabled(true);
		}
		if(no_egs >=1)
		{
			i->setText(ips.at(0));
			o->setText(ops.at(0));
		}
	}
}
void io :: init ()
{
	compiled=false;
	GetPath();
	if(ReadFile(path_io+".mode")=="egs")
	{
		mode=1;
		lblper->hide();
		p->hide();
		lbltt->hide();
		letime->hide();
		lblms->hide();
		distr->hide();
	}
	if(ReadFile(path_io+".mode")=="cases")
		mode=0;
	QFile(path_io+".mode").remove();
	LoadIOP();
	ChangeGUI();
}
void io::SaveCurrent()
{
	if(mode==1)
	{
		ips.at(egs_index)=i->text();
		ops.at(egs_index)=o->text();
	}
	if(mode==0)
	{	
		ips.at(case_index)=i->text();
		ops.at(case_index)=o->text();
		pers.at(case_index)=QString::number(p->value());
	}	
}
void io::LoadNew()
{
	if(mode)
	{	
		i->setText(ips.at(egs_index));
		o->setText(ops.at(egs_index));
	}
	else
	{	
		i->setText(ips.at(case_index));
		o->setText(ops.at(case_index));
		p->setValue(pers.at(case_index).toInt());
	}	
		
}
void io::Prev_clicked()
{
	letime->clear();
	SaveCurrent();
	if(mode)
	{
		if(egs_index >0)
		{
			egs_index--;
			ChangeGUI();
			LoadNew();
		}
	}
	else
	{
		if(case_index >0)
		{
			case_index--;
			ChangeGUI();
			LoadNew();
		}
	}	
}

void io::ChangeGUI()
{
	if(mode ==0)
	{	
		lblcase->setText("Case "+QString::number(case_index+1)+"/"+QString::number(no_cases));
		if(case_index > 0 )
			Prev->setEnabled(true);
		else
			Prev->setEnabled(false);
		if(case_index < no_cases-1)
			Next->setEnabled(true);
		else
			Next->setEnabled(false);
		if(no_cases==1)
			Sub->setEnabled(false);
		else
			Sub->setEnabled(true);
	}
	if(mode==1)
	{
		lblcase->setText("Example "+QString::number(egs_index+1)+"/"+QString::number(no_egs));
		
		if(egs_index > 0 )
			Prev->setEnabled(true);
		else
			Prev->setEnabled(false);
		if(egs_index < no_egs-1)
			Next->setEnabled(true);
		else
			Next->setEnabled(false);
		if(no_egs==1)
			Sub->setEnabled(false);
		else
			Sub->setEnabled(true);
	}
}
void io::Next_clicked()
{
	SaveCurrent();
	letime->clear();
	if(!mode)
	{	
		if(case_index <= no_cases-2)
		{
			case_index++;
			ChangeGUI();
			LoadNew();
		}	
	}	
	else
	{
		if(egs_index <= no_egs-2)
		{
			egs_index++;
			ChangeGUI();
			LoadNew();
		}
	}
}

void io::New_clicked()
{
	SaveCurrent();
	if(mode==0)
	{	
		no_cases ++ ;
		ips.resize(no_cases);
		ops.resize(no_cases);
		pers.resize(no_cases);
		case_index = no_cases-1;
		i->clear();
		o->clear();
		p->setValue(1);
	}
	if(mode==1)
	{
		no_egs ++ ;
		ips.resize(no_egs);
		ops.resize(no_egs);
		egs_index = no_egs-1;
		i->clear();
		o->clear();
	}
	ChangeGUI();
	letime->clear();
}

void io::Sub_clicked()
{
	if(QMessageBox::warning(this, "Warning", "Are U Sure ??", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
			return;
	if(mode==0)
	{
		ips.erase  (ips.begin( )+case_index);
		ops.erase(ops.begin( )+case_index);
		pers.erase(pers.begin( )+case_index);
		no_cases=ips.size();
		if(case_index == no_cases)
			case_index--;
	}
	if(mode==1)	
	{
		ips.erase  (ips.begin( )+egs_index);
		ops.erase(ops.begin( )+egs_index);
		no_egs=ips.size();
		if(egs_index == no_egs)
			egs_index--;
	}
	ChangeGUI();
	LoadNew();
	letime->clear();
}

void io::Done_clicked()
{
	SaveCurrent();
	if(mode==0)
	{	
		int per=0;
		for(int i=0;i<no_cases;i++)
			per += pers.at(i).toInt();
		if(per != 100 )
		{
				MsgBox("Error","Sum Of %s != 100");
				return ;
		}
	}	
	if(QMessageBox::warning(this, "Warning", "Are You Sure?? ", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
			return;
	if(mode==0)
	{	
		QString cases=TST+"\n"+QString::number(tlsb->value())+"\n"+TET+"\n";
		for( int i=0;i<no_cases;i++)
		{
			cases += IST+"\n"+ips.at(i)+"\n"+IET;
			cases += "\n"+OST+"\n"+ops.at(i)+"\n"+OET+"\n";
			cases += PST+"\n"+pers.at(i)+"\n"+PET+"\n";
		}
		WriteFile(path_io+pname+"test",cases);
	}
	if(mode==1)
	{
		QString egs=TST+"\n"+QString::number(tlsb->value())+"\n"+TET+"\n";
		for( int i=0;i<no_egs;i++)
		{
			egs += IST+"\n"+ips.at(i)+"\n"+IET;
			egs += "\n"+OST+"\n"+ops.at(i)+"\n"+OET+"\n";
		}
		WriteFile(path_io+"."+pname+"e",egs);
	}	
	
	QFile(path_io+"a.out").remove();
	this->close();
}
void io::OnCExit()
{
	if(!QFile::exists(path_io+"a.out"))
	{
		MsgBox("Error","The Code Didn't Compile Successfully");
		Detect->setText("Detect Output");
		return ;
	}
	else
	{
		compiled=true;
		Detect->setText("Detect Output");
		DetectOP();
	}	
}

void io::OnExit()
{
	o->setText(QString(proc->readStdout()).stripWhiteSpace());
	ulong tt=ToMSeconds(QTime::currentTime())-st;
	letime->setText(QString::number(tt));
	Detect->setText("Detect Output");
}

ulong io::ToMSeconds(QTime t )
{
	return (ulong(( t.hour()*3600000 + t.minute()*60000 + t.second()*1000 + t.msec() )));
}
void io::DetectOP()
{
	Detect->setText("Executing ... ");
	if(proc!=NULL)
		delete proc;
	proc=new QProcess(this);
	proc->setWorkingDirectory(path_io);
	proc->addArgument ( "./a.out");
	connect( proc, SIGNAL( processExited()),this, SLOT(OnExit()) );
	if(!proc->launch(i->text().stripWhiteSpace()))
	{
		MsgBox("Error","Error Starting process");
		Detect->setText("Detect Output");
	}
	else
		st = ToMSeconds(QTime::currentTime());
}
void io::Detect_clicked()
{
	if(!compiled)
	{
		QFile(path_io+"a.out").remove();
		Detect->setText("Compiling ... ");
		Detect->repaint(true);
		QProcess* c=new QProcess(this);
		c->setWorkingDirectory(path_io);
		c->addArgument ( "g++");
		c->addArgument(pname+".cpp");	
		connect( c, SIGNAL( processExited()),this, SLOT(OnCExit()) );
		if(!c->start())
		{
			MsgBox("Error","Error Compiling Code");
			Detect->setText("Detect Output");
		}
	}	
	else
		DetectOP();
}


void io::distr_clicked()
{
	if(mode == 0)
	{
	              SaveCurrent();
		int avg = 100/no_cases ;
		for(uint i=0;i<pers.size();i++)
			pers.at(i)=QString::number(avg) ;
		if(avg*no_cases != 100)
			pers.at(pers.size()-1) =QString::number(pers.at(pers.size()-1).toInt()+100-avg*no_cases) ;
		LoadNew();
	}
}


