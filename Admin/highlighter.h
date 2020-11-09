#include <qsyntaxhighlighter.h>
#include <qstring.h>
#include <vector>
#include <qstring.h>
#include <qmessagebox.h>
#include <qdir.h>

using namespace std ;

class SyntaxHighlighter : public QSyntaxHighlighter
{
	vector < pair < QString,QString> > keywords ;
	
	public:
	int fsize;
	QFont normal_font;
	QColor normal_color;
	QFont keyword_font;
	
	SyntaxHighlighter(QString path, QTextEdit* textEdit) : QSyntaxHighlighter(textEdit)
	{
		fsize=textEdit->currentFont().pointSize();
		normal_font = QFont("Luxi Mono",fsize,QFont::Normal);
		normal_font.setStyleHint ( QFont::AnyStyle, QFont::PreferAntialias ) ;
		normal_color= QColor("black");
		
		keyword_font = QFont("Luxi Mono",fsize,QFont::Bold);
		keyword_font.setStyleHint ( QFont::AnyStyle, QFont::PreferAntialias ) ;
		
		QString k = ReadFile(path) ;
		QStringList kws = QStringList::split("\n",k);
		QStringList :: Iterator kwi = kws.begin();
		
		while(kwi != kws.end())
		{
		    QString temp = *kwi ;
		    pair < QString , QString > kwac ;
		    QStringList ttt = QStringList::split(" ",temp,false);
		    QStringList :: Iterator g = ttt.begin() ;
		    kwac.first=(*g).stripWhiteSpace();g++ ;
		    kwac.second = (*g).stripWhiteSpace();
		    keywords.push_back(kwac);kwi ++ ;
		}
		QFile(path).remove();
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
	int highlightParagraph(const QString& text, int state)
	{
		setFormat( 0, text.length() ,normal_font,normal_color );
		HighlightKeyWords(text);
		return state ;
	}	
} *q ;

