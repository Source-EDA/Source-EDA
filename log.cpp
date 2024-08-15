#include "log.hpp"
#include <QString>
#include <QMessageBox>
#include <iostream>

QtMessageHandler Log::originalHandler = nullptr;
QMainWindow * Log::main = nullptr;
std::list<LogPopup *> Log::logs = std::list<LogPopup *>();

void Log::log(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString prefix;
    switch (type) {
        case QtCriticalMsg:
            prefix = "ERROR";
            break;
        case QtFatalMsg:
            prefix = "FATAL";
            break;
        case QtInfoMsg:
            prefix = "INFO";
            break;
        case QtDebugMsg:
            prefix = "DEBUG";
            break;
        case QtWarningMsg:
        default:
            prefix = "WARNING";
    }

    if(type != QtDebugMsg) {
        LogPopup* msgPopup = new LogPopup(main);
        // msgPopup.setIconPixmap(QIcon::fromTheme(prefix).pixmap(32));
        logs.push_back(msgPopup);
        
        msgPopup->setWindowTitle(prefix);
        msgPopup->setText("[" + prefix + "] " + msg);
        msgPopup->setTitle(prefix);
        msgPopup->setObjectName("log");
        msgPopup->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint );
        updatePosition();
        msgPopup->show();
    }

    if (originalHandler)
        originalHandler(type, context, msg);
}

void Log::setUp(QMainWindow * mainWin) {
    originalHandler = qInstallMessageHandler(log);
    main = mainWin;
}

void Log::updatePosition() {
    auto it = logs.begin();
    for(int i = 0; i < logs.size() ; i++) {
        int x = main->width() - (*it)->width() - 10;
        int y = main->height() - (((*it)->height() + 10) * (i+1));
        (*it)->move( x, y);
        std::advance(it, 1);
    } 
}

void Log::closePopup(LogPopup * popup) {
    std::list<LogPopup *>::iterator it = std::begin(logs);
    while(*(it) != popup) 
        std::advance(it, 1);
    logs.erase(it);
    delete popup;
    updatePosition();
}
