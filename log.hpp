#ifndef LOG_H
#define LOG_H

#include <QtLogging>
#include <QMainWindow>
#include <list>
#include "logpopup.hpp"


class Log {

public:
    static void log(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static void setUp(QMainWindow * mainWin);
    static void updatePosition();
    static void closePopup(LogPopup * popup);

private:
    static QMainWindow * main;
    static QtMessageHandler originalHandler;
    static std::list<LogPopup *> logs;

};


#endif // LOG_H