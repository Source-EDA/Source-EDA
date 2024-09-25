#include "projectManager/sourceeda.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SourceEDA mumWindow;
    QFile styleFile( ":/resources/stylesheet.qss" );
    styleFile.open( QFile::ReadOnly );
    a.setStyleSheet(QString(styleFile.readAll()));

    mumWindow.show();
    return a.exec();
}
