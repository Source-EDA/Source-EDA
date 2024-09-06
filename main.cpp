#include "projectManager/sourceeda.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SourceEDA mumWindow;
    mumWindow.show();
    return a.exec();
}
