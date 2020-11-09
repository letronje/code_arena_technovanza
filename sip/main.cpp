#include <qapplication.h>
#include "sip.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    sip w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
