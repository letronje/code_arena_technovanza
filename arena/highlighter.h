#include <qsyntaxhighlighter.h>
#include <qstring.h>
#include <vector>
#include <qstring.h>
#include <qmessagebox.h>

#define INCOMMENT 		1
#define NOTINCOMMENT	0
#define FIRSTPARA 		-2

using namespace std ;
bool flag = false;

class SyntaxHighlighter : public QSyntaxHighlighter
{
	vector < pair < QString,QString> > keywords ;
	QTextEdit * te;
	QString kpath ;
	
	public:
	QFont normal_font;
	QColor normal_color;
	QFont keyword_font;
	QFont comment_font;
	QColor comment_color;
	void LoadKeyWords()
	{
		QString k;
		while((k=ReadFile(kpath))=="-1")
		{	
			int opt = QMessageBox::critical( 0, "Error","Error Loading KeyWords","Retry","Skip", 0, 0, 1 ) ;
			switch(opt)
			{
				case 0: break ;
				case 1: flag=true;goto ok;
			}	
		}
		ok:
		QStringList kws = QStringList::split("\n",k);
		QStringList :: Iterator kwi = kws.begin();
		keywords.clear();
		while(kwi != kws.end())
		{
			QString temp = *kwi ;
			if(temp.find("//")!=-1)
			{
				kwi++ ;
				continue ;
			}	
			pair < QString , QString > kwac ;
			QStringList ttt = QStringList::split(" ",temp,false);
			QStringList :: Iterator g = ttt.begin() ;
			kwac.first=(*g).stripWhiteSpace();g++ ;
			kwac.second = (*g).stripWhiteSpace();
			keywords.push_back(kwac);
			kwi ++ ;
		}
	}
	SyntaxHighlighter(QString path, QTextEdit *textEdit) : QSyntaxHighlighter(textEdit)
	{
		kpath=path ;
		int fsize = textEdit->currentFont().pointSize ();
		normal_font = QFont("Luxi Mono",fsize,QFont::
							Normal);
		normal_font.setStyleHint ( QFont::AnyStyle, QFont::PreferAntialias ) ;
		normal_color= QColor("black");
		
		comment_font = QFont("Luxi Mono",fsize,QFont::Bold);
		comment_font.setStyleHint ( QFont::AnyStyle, QFont::PreferAntialias ) ;
		comment_color= QColor("darkGreen");
		
		keyword_font = QFont("Luxi Mono",fsize,QFont::Bold);
		keyword_font.setStyleHint ( QFont::AnyStyle, QFont::PreferAntialias ) ;
		
		te = textEdit; 
		LoadKeyWords();
	}
	QString ReadFile ( QString fname ) 
	{
		QString data ;
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
			return data ;
		}
		else
			data = "-1";
		return data;
	}
	void HighlightKeyWords(const QString& text)
	{
		for ( uint kwctr = 0 ; kwctr < keywords.size() ; kwctr ++ )
		{
			QString kw = keywords.at(kwctr).first ;
			QString kwc = keywords.at(kwctr).second ;
			QColor c (kwc);
			
			if ( kwc.find(',',0) >= 0 )
			{
				QStringList rgb = QStringList::split(",",kwc,false);
				QStringList::Iterator h = rgb.begin();
				int r,g,b ;
				r=(*h).toInt();h++;
				g=(*h).toInt();h++;
				b=(*h).toInt();
				c.setRgb(r,g,b);
			}
			
			int start=0;
			int index ;
			
			while( ( index = text.find(kw,start) ) != -1 )
			{	
				bool left= false;
				bool right = false ;
				
				if(index == 0 || ( index >= 1 && IsValid(text.at(index-1))))
				    left=true ;	
				if ( (index+kw.length() == text.length() ) || ( (index+kw.length() < text.length() ) &&  IsValid(text.at(index+kw.length()))))
					right=true ;	
				if( ( left && right ) || ( kw.length() == 1 ))
					setFormat(index,kw.length(),keyword_font,c);	
				start = index+kw.length()+1;
			}		
		}
	}
	
	bool IsValid ( char c )
	{
		if(c>='0' && c<='9' ) return false ;
		if(c>='a' && c<='z' ) return false ;
		if(c>='A' && c<='Z' ) return false ;
		return true ;
	}
	void HighlightSingleLineComment(const QString& text)
	{
		int index = -1;
		if((index = text.find("//"))!= -1)
			setFormat(index,text.length()-index,comment_font,comment_color);
	}
	
	int HighlightMultiLineComment(const QString& text ,int state)
	{
		int start=0;
		int sindex,eindex ;
		int retval =NOTINCOMMENT;
		if(state == FIRSTPARA || state == NOTINCOMMENT)
			goto again ;
		else if(state == INCOMMENT)
		{
			start=0;
			
			eindex = text.find("*/",start);
			
			if(eindex == -1 )
			{
				setFormat(0,text.length(),comment_font,comment_color);
				retval= INCOMMENT ;
				goto end;
			}	
			else
			{
				setFormat(0,eindex+2,comment_font,comment_color);
				start=eindex+2;
			}
			
			again:
			
			sindex = text.find("/*",start);
			if(sindex == -1 )
			{
				retval= NOTINCOMMENT ;
				goto end;
			}	
			else
			{
				if(uint(sindex+2)<=text.length()-2)
				{
					if((eindex=text.find("*/",sindex+2)) == -1)
					{
						setFormat(sindex,text.length()-sindex,comment_font,comment_color);
						retval =INCOMMENT ;	
						goto end ;
					}
					else
					{
						setFormat(sindex,eindex-sindex+2,comment_font,comment_color);
						start = eindex+2;
						goto again ;
					}
				}
				else
				{
					setFormat(sindex,text.length()-sindex,comment_font,comment_color);
					retval= INCOMMENT ;
					goto end;
				}
			}	
		}
		end:
		return retval;
	}//virtual ~SyntaxHighlighter(void);
	int highlightParagraph(const QString& text, int state)
	{
		setFormat( 0, text.length() ,normal_font,normal_color );
		HighlightKeyWords(text);
		HighlightSingleLineComment(text);
		return HighlightMultiLineComment(text,state);
	}	
} *q ;

