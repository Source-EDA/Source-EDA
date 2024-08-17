#ifndef LOG_H
#define LOG_H

#include <QPlainTextEdit>
#include <QtLogging>

class Log {
public:
    static void setup(QPlainTextEdit * plainTextEdit);
    static void write(QtMsgType level, QString context, QString title, QString text);
    static void write(QtMsgType level, QString context, QString title);

private:
    static QPlainTextEdit * logZone;
    static void write_(QString logMsg);
};


#endif //LOG_H