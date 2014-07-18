/****************************************************************/
/*Author:                                                       */
/*	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     */
/*				      <e.yllanescucho@gmail.com>                */
/****************************************************************/

#include "MainWindow.h"

// Qt header files
#include <QtGui/QApplication>
#include <QtGui>
#define X_INITIAL 0
#define Y_INITIAL 0

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationVersion(QUOTE(APP_VERSION));
    MainWindow w;
    w.show();
    w.setGeometry(X_INITIAL, Y_INITIAL, w.width(), w.height());
    return a.exec();
} // main()
