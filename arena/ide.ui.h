#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <qdir.h>
#include <qtimer.h>
#include <qstring.h>
#include <qdatetime.h> 
#include <qfile.h>
#include <qstringlist.h> 
#include <qmessagebox.h>
#include <qevent.h>
#include <unistd.h>
#include <qprocess.h>
#include <qprogressdialog.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <qapplication.h>

#include "options.h"
#include "highlighter.h"
#include "welcome.h"
#include "score.h"
#include "result.h"
#include "rules.h"
#include "test.h"
#include "decl.h"

using namespace std ;

QString path_ide , prevprob, codepath , prob , compiled_code ;
QProcess* r =NULL;
QProcess* sp= NULL ;
QProcess* subp=NULL ;
QString Theme ;
int AT ,TW ,W , WaE, init_code_time=0 ,tct=0;
FILE* fc ;
bool can_sub=true , show_score =true ;
int start_time , end_time , compile_success = 0 , code_start_time =0;
QTimer* t =NULL ;//timer  used to show hrs,mins&secs remaining
QTimer* subtimer=NULL ;

void ide::LoadTheme()
{
	if(Theme=="Rainbow")
	{
		this->setPaletteBackgroundColor(QColor("#e6e6e6"));
		
		lblca->setFrameShape(QFrame::Box);
		lblca->setPaletteBackgroundColor(QColor("#d0ceff"));
		
		team->setFrameShape(QFrame::Box);
		team->setPaletteBackgroundColor(QColor("#cfe6de"));
		team->setPaletteForegroundColor(QColor("#005500"));
				
		lbltime->setPaletteBackgroundColor(QColor("#ffc5fc"));
		lbltime->setPaletteForegroundColor(QColor("#aa007f"));
		
		lblscore->setPaletteBackgroundColor(QColor("#840000"));
		lblscore->setPaletteForegroundColor(QColor("#c3c300"));
		
		lblprob->setPaletteBackgroundColor(QColor("#e6e6e6"));
		lblprob->setPaletteForegroundColor(QColor("#55007f"));
		
		linenum->setPaletteBackgroundColor(QColor("#e6e6e6"));
		linenum->setPaletteForegroundColor(QColor("#550000"));
				
		Editor->setPaletteBackgroundColor(QColor("#bfd2e8"));
		Editor->setPaletteForegroundColor(QColor("#000000"));
		
		Problem->setPaletteBackgroundColor(QColor("#bcd3bb"));
		Problem->setPaletteForegroundColor(QColor("#000000"));
		
		Compiler->setPaletteBackgroundColor(QColor("#c9bdd3"));
		Compiler->setPaletteForegroundColor(QColor("#000000"));
		
		Rules->setPalette(QPalette(QColor("#aaaaff"),QColor("#e6e6e6")));
		Refresh->setPalette(QPalette(QColor("#32c681"),QColor("#e6e6e6")));
		Options->setPalette(QPalette(QColor("#ffd3f1"),QColor("#e6e6e6")));
		Compile->setPalette(QPalette(QColor("#ffea49"),QColor("#e6e6e6")));
		Test->setPalette(QPalette(QColor("#4fcaff"),QColor("#e6e6e6")));
		Submit->setPalette(QPalette(QColor("#e9ffc7"),QColor("#e6e6e6")));
		Score->setPalette(QPalette(QColor("#ffb592"),QColor("#e6e6e6")));
		Result->setPalette(QPalette(QColor("#ff94ad"),QColor("#e6e6e6")));
		Close->setPalette(QPalette(QColor("#aaaa7f"),QColor("#e6e6e6")));
		
		Rules->setPaletteForegroundColor(QColor("#000000"));
		Refresh->setPaletteForegroundColor(QColor("#000000"));
		Options->setPaletteForegroundColor(QColor("#000000"));
		Compile->setPaletteForegroundColor(QColor("#000000"));
		Test->setPaletteForegroundColor(QColor("#000000"));
		Score->setPaletteForegroundColor(QColor("#000000"));
		Result->setPaletteForegroundColor(QColor("#000000"));
		Close->setPaletteForegroundColor(QColor("#000000"));
		Submit->setPaletteForegroundColor(QColor("#000000"));	
	}
	else if(Theme=="Blue_Lagoon")
	{
		this->setPaletteBackgroundColor(QColor("#d8d8e6"));
		
		lblca->setFrameShape(QFrame::ToolBarPanel);
		
		team->setFrameShape(QFrame::ToolBarPanel);
		
		lbltime->setPaletteBackgroundColor(QColor("#d8d8e6"));
		lbltime->setPaletteForegroundColor(QColor("#550000"));
		
		lblscore->setPaletteBackgroundColor(QColor("#d8d8e6"));
		lblscore->setPaletteForegroundColor(QColor("#005500"));
		
		lblprob->setPaletteBackgroundColor(QColor("#d8d8e6"));
		lblprob->setPaletteForegroundColor(QColor("#55007f"));
		
		linenum->setPaletteBackgroundColor(QColor("#d8d8e6"));
		linenum->setPaletteForegroundColor(QColor("#550000"));
			
		Editor->setPaletteBackgroundColor(QColor("#efefff"));
		Problem->setPaletteBackgroundColor(QColor("#efefff"));
		Compiler->setPaletteBackgroundColor(QColor("#efefff"));
	
		Rules->setPalette(QPalette(QColor("#95a6e6"),QColor("#d8d8e6")));
		Refresh->setPalette(QPalette(QColor("#95a6e6"),QColor("#d8d8e6")));
		Options->setPalette(QPalette(QColor("#95a6e6"),QColor("#d8d8e6")));
		Compile->setPalette(QPalette(QColor("#95a6e6"),QColor("#d8d8e6")));
		Test->setPalette(QPalette(QColor("#95a6e6"),QColor("#d8d8e6")));
		Submit->setPalette(QPalette(QColor("#95a6e6"),QColor("#d8d8e6")));
		Score->setPalette(QPalette(QColor("#95a6e6"),QColor("#d8d8e6")));
		Result->setPalette(QPalette(QColor("#95a6e6"),QColor("#d8d8e6")));
		Close->setPalette(QPalette(QColor("#95a6e6"),QColor("#d8d8e6")));
		
		Rules->setPaletteForegroundColor(QColor("#00007f"));
		Refresh->setPaletteForegroundColor(QColor("#00007f"));
		Options->setPaletteForegroundColor(QColor("#00007f"));
		Compile->setPaletteForegroundColor(QColor("#00007f"));
		Test->setPaletteForegroundColor(QColor("#00007f"));
		Score->setPaletteForegroundColor(QColor("#00007f"));
		Result->setPaletteForegroundColor(QColor("#00007f"));
		Close->setPaletteForegroundColor(QColor("#00007f"));
		Submit->setPaletteForegroundColor(QColor("#00007f"));	
		
		
	}
}
void ide::Options_clicked()
{
	options* o = new options(this);
	o->exec();
	LoadOptions();
	LoadTheme();
}

void ide :: GetPaths()
{
	GetPath(path_ide);
	codepath = QDir::homeDirPath()+"/arena/" ;
}

void ide::Compile_clicked()
{
	Refresh->setEnabled(false);
	Submit->setEnabled(false);
	Score->setEnabled(false);
	Test->setEnabled(false);
	Compiler->setEnabled(false);
	Result->setEnabled(false);
	
	LoadOptions();
	Compiler->clear();
	QFile(path_ide + "test" ).remove();
	Compiler->append("Compiling " + prob + ".cpp .....") ;
	this->setCursor( Qt::WaitCursor );  
	QString command = "g++ -Wno-deprecated " ;
	if ( W == 0  )
		command += "-w ";
	else
	{
		command += "-Wall ";
		if ( WaE == 1 )
			command +="-Werror ";
	}
	command +="-o ";
	command += path_ide +"test ";
	command += codepath+prob+".cpp >& "+path_ide+".co";
	system(command);
	Compiler->clear();
	QString cmsg = ReadFile(path_ide+".co");
	int status;
	if(QFile::exists(path_ide+"test"))
	{
		if(cmsg=="") 
		{
			Compiler->append("<font color=\"darkGreen\">Your code compiled Successfully.</font>");
			compile_success = 1 ;
			compiled_code = Editor->text();
			chmod(path_ide+"test",S_IRUSR|S_IWUSR| S_IXUSR);
			status=0;
		}
		else
		{
			Compiler->append("<font color=\"#c3420f\">Your Code Compiled Successfully With Following Warnings</font>");
			Compiler->append("\n");
			compile_success = 1 ;
			compiled_code = Editor->text();
			status=1;
		}
	}
	else
	{
		compile_success=0;
		Compiler->append("<font color=\"darkRed\">Your Code Compiled With Following Errors</font>");
		status=2;
		Compiler->append("\n");
		if(cmsg.find("cc1plus")!=-1)
			cmsg.replace("cc1plus","<font color=\"#55007f\">Note</font>");
	}
	cmsg.replace(codepath,"");
	if(status==1)
		cmsg = "<font color=\"#c3420f\">"+cmsg+"</font>";
	if(status==2)
		cmsg = "<font color=\"darkRed\">"+cmsg+"</font>";
	Compiler->append (cmsg.replace("\n","<br>"));
	this->setCursor(Qt::ArrowCursor);  
	if(cmsg != "")
		Compiler->append("\n");
	Test->setEnabled(true);
	if ( AT == 1 && QFile::exists(path_ide+"test"))
		Test_clicked( ) ;
	QFile(path_ide+".co").remove();
	Refresh->setEnabled(true);
	Submit->setEnabled(true);
	Score->setEnabled(true);
	Test->setEnabled(true);
	Compiler->setEnabled(true);
	Result->setEnabled(true);
	
}

void ide::closeEvent(QCloseEvent* ce)
{	
	this->show();
	if( t!=NULL && t-> isActive ())
		t->stop();
	welcome* w =new welcome;
	w->show();
	ce->accept();
}	

void ide::Close_clicked()
{
    if(QMessageBox::warning(this, "Warning", "Are You Sure you wanna close the Coding Window ??. The timer for this problem will keep on ticking till you submit it ", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
		return ;
    this->close();
}

QString ide :: GetCurrTime()
{
	QTime ct = QTime :: currentTime();
	return QString::number(ct.hour())+" "+QString::number(ct.minute())+" "+QString::number(ct.second());
}
void ide::start_time_load()
{
	if ( !QFile::exists(path_ide+".stime"))
	{
		WriteFile(path_ide+".stime",GetCurrTime());
		start_time = ToSeconds(QTime :: currentTime());
	}	
	else
	{	
		QString stime= ReadFile(path_ide+".stime") ;
		if(stime.length() < 3)
		{
			stime=GetCurrTime();
			WriteFile(path_ide+".stime",stime);
		}
		QStringList s = QStringList :: split(" ",stime);
		QStringList::Iterator it = s.begin();
		start_time = (*it).toUInt() * 3600 ;
		it++;	
		start_time += (*it).toUInt() * 60 ;
		it++ ;	
		start_time += (*it).toUInt();
	}
}

//calculates ending time
void ide::end_time_load()
{
	QString temp ;
	if ( QFile::exists(path_ide+".ttime"))
	{		
		texists:
		while((temp=ReadFile(path_ide+".ttime"))=="-1")
		{	
			int opt = QMessageBox::critical( this, "Error","Error Reading File","Retry","Quit", 0, 0, 1 ) ;
			switch(opt)
			{
				case 0: break ;
						case 1: exit(1);
							}
		}
		QStringList str = QStringList :: split(" ",temp);
		QStringList::Iterator it = str.begin();
		ulong t_time ;
		if((*it).toUInt()>3)
		{
			t_time = 3*3600 ;
			goto SKIP;
		}	
		else
			t_time = (*it).toUInt() * 3600 ;
		it++;
		if((*it).toUInt()>59)
			t_time += 59*60 ;
		else
			t_time += (*it).toUInt() * 60 ;
		it++ ;	
		if((*it).toUInt()>59)
			t_time += 59 ;
		else
			t_time += (*it).toUInt() ;
		SKIP:
		end_time = start_time + t_time ;
	}
	else
	{
		while(!QFile::exists(path_ide+".ttime"))
		{	
			int opt = QMessageBox::critical( this, "Error","Required File DoesNot Exist","Retry","Quit", 0, 0, 1 ) ;
			switch(opt)
			{
				case 0: break ;
						case 1: exit(1);
							}
		}
		goto texists;
	}
}

void ide::EXIT()
{
	WriteFile(path_ide+".sub","0");
	if(ReadFile(path_ide+".shown")!="1")
	{
		MsgBoxI("Time Up","Your Time Is Up.You can No Longer Submit Codes");
		WriteFile(path_ide+".shown","1");
	}	
}
void ide::update_time()
{
	if(show_score)
	{
		int tpt = init_code_time+ToSeconds(QTime::currentTime())-code_start_time;
		lblscore->setText(QString::number(prob.toDouble()*(0.3+(0.7*tct*tct)/((10.0*double(tpt)*double(tpt))+tct*tct)),'f',4));
	}
	LoadOptions();
	int current_time = ToSeconds(QTime::currentTime());
	int remaining_time = end_time - current_time ;
	if(remaining_time < 0)
		remaining_time=0;
	uint hrs  = remaining_time / 3600; 
	uint mins = ( remaining_time - ( hrs * 3600 )  ) / 60;
	uint secs = remaining_time - ( hrs * 3600 ) - ( mins * 60 ) ;
	QString h,m,s;
	h  = QString::number ( hrs ) ;
	m = QString::number ( mins ) ;
	s  = QString::number ( secs ) ;
	if ( h.length()==1)
		h.prepend("0");
	if ( m.length()==1)
		m.prepend("0");
	if ( s.length()==1)
		s.prepend("0");
	lbltime->setText( h+":"+m+ ":"+s);
	if ( current_time != start_time )
	{
		if ( TW == 1 )
			if ( (current_time - start_time ) % 1800 == 0 )
				MsgBoxW("Hurry Up","Time Remaining: " + lbltime->text());
		if ( TW == 2 )
			if ( (current_time - start_time ) % 3600 == 0 )
				MsgBoxW("Hurry Up","Time Remaining: " + lbltime->text());
	} 
	if ( remaining_time == 5*60 )
		MsgBoxW("Hurry Up","Last 5 Minutes Remaining");
	if ( remaining_time <= 0 )
	{
		t->stop();
		if(sp!=NULL)
			delete sp;
		sp=new QProcess(this);
		sp->setWorkingDirectory(path_ide);
		sp->addArgument ( path_ide+"./client");
		sp->addArgument("8");
		sp->addArgument(ReadFile(path_ide+".team"));
		connect( sp, SIGNAL( processExited()),this, SLOT(EXIT()) );
		sp->start();
	}
}
void ide::start_timers()
{
	connect( t, SIGNAL(timeout()), SLOT(update_time()) );
	t->start( 1000, FALSE );
}
bool ide::problem_name_load()
{
	if((prob= ReadFile(path_ide+".p")) == "-1")
		return false;
	else
	{
			lblprob->setText(prob);
			return true ;
		}	
}
void ide::problem_load ( )
{
	QString p ;
	if(QFile::exists(path_ide+prob +"p"))
	{
		exists:
		while((p=ReadFile(path_ide+prob +"p"))== "-1") 
		{
			int opt = QMessageBox::critical( this, "Error","Error Reading Problem "+prob,"Retry","I Remember It.", 0, 0, 1 ) ;
			switch(opt)
			{
				case 0: break ;
				case 1: p="Error Loading Problem";goto blankprob;
			}
		}
		blankprob:
		Problem->setText(p);
	}
	else
	{
		while(!QFile::exists(path_ide+prob +"p")) 
		{
			int opt = QMessageBox::critical( this, "Error","Problem "+prob+"Does Not Exist","Retry","I Remember It", 0, 0, 1 ) ;
			switch(opt)
			{
				case 0: break;
				case 1: Problem->setText("Error Loading Problem");return ;
			}
		}
		goto exists;
	}
}

void ide::code_load()
{
	QString code ;
	if(QFile::exists(codepath+prob+".cpp"))
	{
		while((code = ReadFile(codepath+prob+".cpp")) == "-1")
		{
			int opt = QMessageBox::critical( this, "Error","Error Loading "+prob+".cpp","Retry","Cancel", 0, 0, 1 ) ;
			switch(opt)
			{
				case 0: break ;
				case 1: code="";
			       	     goto blankcode;
			}
		}
		blankcode:
		Editor->setText(code);
	}
	else
	{
		QString deft = "//This is just the default template, you can remove it if you want.\n\n#include <iostream>\n\n";
		deft +=  "using namespace std ;\n\nint main ()\n{\n\treturn 0 ;\n}" ;
		WriteFile(codepath+prob+".cpp",deft);
		Editor->setText(deft);
	}
	Editor->setTabStopWidth(20);
}	
void ide :: LoadDefaultOptions()
{
	AT=1;
	TW =1;
	W =1;
	WaE=0 ;
	QString options="0\n1\n1\n0\n";
	while(!WriteFile(path_ide+".options",options))
	{	
		int opt = QMessageBox::critical( this, "Error","Error Saving Default Options","Retry","Quit", 0, 0, 1 ) ;
		switch(opt)
		{
			case 0: break ;
			case 1: exit(1);
		}		
	}
}

void ide :: init()
{
	GetPaths();	
	start_time_load();
	end_time_load();
	QTime cct = QTime::currentTime();
	long current_time = ToSeconds(cct);
	long remaining_time = end_time - current_time ;
	if(remaining_time <= 0)
	{
		if(sp!=NULL)
			delete sp;
		sp=new QProcess(this);
		sp->setWorkingDirectory(path_ide);
		sp->addArgument ( path_ide+"./client");
		sp->addArgument("8");
		sp->addArgument(ReadFile(path_ide+".team"));
		connect( sp, SIGNAL( processExited()),this, SLOT(EXIT()) );
		sp->start();
	}
	QFile(path_ide+".sub").remove();
	while(system(path_ide+"./client 9 "+ReadFile(path_ide+".team"))!=0)
	{	
		int opt = QMessageBox::critical( this, "Error","Error Connecting To The Server","Retry","Quit", 0, 0, 1 ) ;
		switch(opt)
		{
			case 0: break ;
			case 1: exit(1);
		}	
	}
	if(ReadFile(path_ide+".sub")=="0")
		can_sub=false;
	team->setText("Team: " + ReadFile(path_ide+".team"));
	LoadOptions();
	LoadTheme();
	prob = ReadFile(path_ide+".p");
	lblprob->setText(prob);
	code_load();
	problem_load();
	compile_success=0;
	//compiled_code_load();
	q = new SyntaxHighlighter(path_ide+".keywords",Editor);
	if(can_sub)
	{	
		while(system(path_ide+"./client 12")!=0)
		{
			int opt = QMessageBox::critical( this, "Error","Error Contacting Server","Retry","Cancel", 0, 0, 1 ) ;
			switch(opt)
			{
				case 0: break ;
				case 1:  lblscore->setText(":-(")  ;
					      show_score = false;
					      goto proceed ;	  
			}
		}	
		
		proceed:
		
		if(show_score)
		{
			code_start_time =ReadFile(path_ide+"st").toInt() ;
			QFile(path_ide+"at").remove();
			
			init_code_time = ReadFile(path_ide+"at").toInt();
			QFile(path_ide+"at").remove();
			
			tct = ReadFile(path_ide+"tt").toInt();
			QFile(path_ide+"tt").remove();
			
		}
		t= new QTimer (this) ;
		update_time();
		start_timers();
	}	
	if(flag)
	{
		QString temp =Editor->text();
		Editor->clear();
		Editor->setText(temp);
	}
}

bool ide::LoadOptions()
{
	QFile fd ( path_ide+".options" ) ;
	if ( fd.open(IO_ReadOnly) )
	{
		QTextStream s ( &fd ) ;
		s >> AT ;
		s >> TW ;
		s >> W ;
		s >> WaE ;
		s >> Theme ;
		fd.close();
		return true ;
	}
	else
		return false;
}

void ide::Test_clicked()
{
	//Test->hide();
	if(!compile_success)
	{
		MsgBoxC("Unsuccessful","You cannot test without compiling successfully");
		Test->show();
	}	
	else
	{
		if ( Editor->text().simplifyWhiteSpace () != compiled_code.simplifyWhiteSpace () )
		{
			int choice = QMessageBox::warning ( this,"Warning","The code has changed after the last successful compilation","Test Previous Code","Cancel",QString::null,0, 1 );	
			if ( choice == 0 )
			{
			    test* z = new test;
			    z->exec();
			}
		}
		else
		{
			test* z = new test;
			z->exec();
		}
	}
	Test->show();	
}
void ide::SExit()
{
	if(!QFile::exists(path_ide+".score"))
	{
		int opt = QMessageBox::critical( this, "Error","Error Contacting Server ","Retry","Cancel", 0, 0, 1 ) ;
		switch(opt)
		{
			case 0: Submit_clicked() ;
			case 1:  return ;
		}
	}
	else
	{
		MsgBoxI("Successfull Submission","Your Code Has Been Submitted For "+ReadFile(path_ide+".score")+" Points");
		QFile(path_ide+".score").remove();
		this->close();
	}	
}

void ide::Submit_clicked()
{
	if(QMessageBox::warning(this, "Warning", "Are You Sure you want to submit your Problem??", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton)== QMessageBox::No)
		return ;
	if(ReadFile(path_ide+".sub")=="0")
	{
		MsgBoxI("No more submissions","Your Are Not Allowed To Submit");
		return ;
	}	
	
	if ( !compile_success )
		MsgBoxI("Compile Unsuccessful","You code must compile successfully before submission");
	else
	{
		if ( Editor->text().simplifyWhiteSpace () != compiled_code.simplifyWhiteSpace () )
			MsgBoxI("Code Change","Your Code Has Changed Since Last Compilation.Please Compile Your Code Successfully And Then Submit.");	
		else
		{
			/*
			QFile(path_ide+".score").remove();
			QString command = path_ide+"./client";
			command += " 5 " + ReadFile(path_ide+".team") + " ";
			command += prob + " ";
			system(command);
			while(!QFile::exists(path_ide+".score"))
			{
				int opt = QMessageBox::critical( this, "Error","Error Contacting Server For Submission","Retry","Cancel", 0, 0, 1 ) ;
				switch(opt)
				{
					case 0: break ;
					case 1: Submit->show(); return ;
				}
				system(command);
			}
			MsgBoxI("Successfull Submission","Your Code Has Been Submitted For "+ReadFile(path_ide+".score")+" Points");
			QFile(path_ide+".score").remove();
			this->close();
			*/
			QFile(path_ide+".score").remove();
			if(subp!=NULL)
				delete r;
			subp=new QProcess(this);
			subp->setWorkingDirectory(path_ide);
			subp->addArgument (path_ide+"./client");
			subp->addArgument("5");
			subp->addArgument(ReadFile(path_ide+".team"));
			subp->addArgument(prob);
			connect( subp, SIGNAL( processExited()),this, SLOT(SExit()) );
			if(!subp->start())
				MsgBoxC("Error","Error Connecting to the Server ");
		}
	}
}

void ide::Editor_cursorPositionChanged( int para )
{
	linenum->setText( QString::number(para+1));
}

void ide::Editor_doubleClicked()
{
	if ( Compiler->isHidden() )
		Compiler->show();
	else
		Compiler->hide();
	if ( Problem->isHidden() )
		Problem->show();
	else
		Problem->hide();
}
void ide::code_save()
{
	QString extra="" ;
	if ( Editor->text()[ Editor->text().length() -1 ] != '\n' )
		extra="\n" ;
	while(!WriteFile(codepath+prob+".cpp",Editor->text()+ extra))
	{	
			int opt = QMessageBox::critical( this, "Error","Error Saving Code","Retry","Cancel", 0, 0, 1 ) ;
			switch(opt)
			{
				case 0: break ;
				case 1: return ;
			}		
	}	
}
void ide::Editor_textChanged()
{
	code_save();
}
void ide::Score_clicked()
{
	//Score->hide();
	score * s = new score(this);
	s->exec();
	//Score->show();
}
void ide::Problem_doubleClicked()
{
	if ( Editor->isHidden() )
		Editor->show();
	else
		Editor->hide();
	if ( Compiler->isHidden() )
		Compiler->show();
	else
		Compiler->hide();
}

void ide::Compiler_doubleClicked()
{
	if ( Editor->isHidden() )
		Editor->show();
	else
		Editor->hide();
	if ( Problem->isHidden() )
		Problem->show();
	else
		Problem->hide();
}

void ide::Result_clicked()
{
	//Result->hide();
	result* r = new result(this);
	r->exec();
	//Result->show();
}
void ide::Exit()
{
	if(QFile::exists(path_ide+ReadFile(path_ide+".p")+"p"))
		Problem->setText(ReadFile(path_ide+ReadFile(path_ide+".p")+"p"));
	else
	{
		MsgBoxC("Error","Error Connecting to the Server ");
		Problem->setText(prevprob);
	}
	//Refresh->show();
}
void ide::Refresh_clicked()
{
	//Refresh->hide();
	if(QMessageBox::warning (this, "Warning", "Are U Sure U Want To Refresh The Problem Statement ??", QMessageBox::Yes ,QMessageBox::No,QMessageBox::NoButton )== QMessageBox::No) 
	{
	//	Refresh->show();
		return ;
	}	
	prevprob = Problem->text();
	QFile(path_ide+ReadFile(path_ide+".p")+"p").remove () ;
	if(r!=NULL)
		delete r;
	r=new QProcess(this);
	r->setWorkingDirectory(path_ide);
	r->addArgument (path_ide+"./client");
	r->addArgument("4");
	r->addArgument(ReadFile(path_ide+".team"));
	r->addArgument(ReadFile(path_ide+".p"));
	r->addArgument("1");
	connect( r, SIGNAL( processExited()),this, SLOT(Exit()) );
	Problem->setText("Refreshing ...");
	if(!r->start())
	{
		MsgBoxC("Error","Error Connecting to the Server ");
		Problem->setText(prevprob);
	//	Refresh->show();
	}	
}

void ide::Rules_clicked()
{
	rules* r = new rules(this);
	r->exec();
}
