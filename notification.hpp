#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QtLogging>
#include <QMainWindow>
#include <list>
#include "notificationpopup.hpp"


class Notification {

public:
    static void sendNotification(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static void setUp(QMainWindow * mainWin);
    static void updatePosition();
    static void closeNotification(NotificationPopup * popup);

private:
    static QMainWindow * main;
    static QtMessageHandler originalHandler;
    static std::list<NotificationPopup *> notifications;

};


#endif // NOTIFICATION_H