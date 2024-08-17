#include "popup.hpp"
#include <QTimer>
#include <QSettings>
#include <QDateTime>
#include <iostream>
#include "log.hpp"

QWidget * Popup::main = nullptr;

void Popup::popup(QtMsgType level, QString context, QString title, QString text) {

    Log::write(level, context, title, text);
    if(main == nullptr) {
        return;
    }

    QMessageBox::Icon icon;
    switch(level) {
    case QtDebugMsg:
        icon = QMessageBox::Information ;
        break;
    case QtInfoMsg:
        icon = QMessageBox::Information ;
        break;
    case QtWarningMsg:
        icon = QMessageBox::Warning;
        break;
    case QtCriticalMsg:
        icon = QMessageBox::Critical;
        break;
    case QtFatalMsg:
        icon = QMessageBox::Critical;
        break;
    }
    QMessageBox * msgBox = new QMessageBox(main);

    msgBox->setInformativeText(text);
    msgBox->setIcon(icon);
    msgBox->setText(title);
    msgBox->setWindowTitle(title);
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->exec();
    delete msgBox;
}

void Popup::setup(QWidget * mainWin) {
    main = mainWin;
 }

// Popup::~Popup()
// {
// }