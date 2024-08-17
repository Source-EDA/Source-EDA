#include "notification.hpp"
#include <QString>
#include <QMessageBox>
#include <iostream>

QtMessageHandler Notification::originalHandler = nullptr;
QMainWindow * Notification::main = nullptr;
std::list<NotificationPopup *> Notification::notifications = std::list<NotificationPopup *>();

void Notification::sendNotification(QtMsgType type, const QMessageLogContext &context, const QString &msg)
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
        NotificationPopup* msgPopup = new NotificationPopup(main);
        // msgPopup.setIconPixmap(QIcon::fromTheme(prefix).pixmap(32));
        notifications.push_back(msgPopup);
        
        msgPopup->setWindowTitle(prefix);
        msgPopup->setText("[" + prefix + "] " + msg);
        msgPopup->setTitle(prefix);
        msgPopup->setObjectName("log");
        msgPopup->setWindowFlags(Qt::Widget | Qt::FramelessWindowHint );
        updatePosition();
        msgPopup->show();
    }

    // if (originalHandler)
    //     originalHandler(type, context, msg);
}

void Notification::setUp(QMainWindow * mainWin) {
    // originalHandler = qInstallMessageHandler(sendNotification);
    main = mainWin;
}

void Notification::updatePosition() {
    auto it = notifications.begin();
    for(int i = 0; i < notifications.size() ; i++) {
        int x = main->width() - (*it)->width() - 10;
        int y = main->height() - (((*it)->height() + 10) * (i+1));
        (*it)->move( x, y);
        std::advance(it, 1);
    } 
}

void Notification::closeNotification(NotificationPopup * popup) {
    std::list<NotificationPopup *>::iterator it = std::begin(notifications);
    while(*(it) != popup) 
        std::advance(it, 1);
    notifications.erase(it);
    delete popup;
    updatePosition();
}
