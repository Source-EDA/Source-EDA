#ifndef LOG_H
#define LOG_H

#include <QtLogging>
#include <QMainWindow>

class Log {

public:
    static void log(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static void setUp(QMainWindow * mainWin);

private:
    static QMainWindow * main;
    static QtMessageHandler originalHandler;

};


#endif // LOG_H