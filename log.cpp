#include "log.hpp"
#include "iostream"
#include <QSettings>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>

QPlainTextEdit * Log::logZone = nullptr;
QFile Log::logFile = QFile();
QDate Log::currentDate = QDate::currentDate();

void Log::setup(QPlainTextEdit * plainTextEdit) {
    logZone = plainTextEdit;
    logZone->setCursor(Qt::BlankCursor);
}

QString msgTypeToString(QtMsgType level) {
    switch(level) {
    case QtDebugMsg:
        return "debug";
    case QtWarningMsg:
        return "warning";
    case QtCriticalMsg:
        return "critical";
    case QtFatalMsg:
        return "fatal";
    case QtInfoMsg:
    default:
        return "info";
    }
}


void Log::write(QtMsgType level, QString context, QString title, QString text) {
    QSettings settings;
    QString logMsg = settings.value("log/formatWithTitle", "{time} [{level}]({context}) {title}: {text}").toString();
    QDateTime currentDate = QDateTime::currentDateTime();
    logMsg. replace("{time}", currentDate.toString("yyyy-MM-ddTHH:mm:ss")).   
            replace("{context}", context).
            replace("{title}", title).
            replace("{text}", text).
            replace("{level}", msgTypeToString(level));
    write_(logMsg);




}

void Log::write(QtMsgType level, QString context, QString text) {
    QSettings settings;
    QString logMsg = settings.value("log/formatWithoutTitle", "{time} [{level}]({context}) {text}").toString();
    logMsg. replace("{time}", QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss")).   
            replace("{context}", context).
            replace("{text}", text).
            replace("{level}", msgTypeToString(level));

    write_(logMsg);

}

void Log::write_(QString logMsg) {
    if(logZone == nullptr) {
        std::cerr << "WARNING : Cannot write log as logZone is null" << std::endl; 
    } else {
        logZone->moveCursor(QTextCursor::End);
        logZone->insertPlainText(logMsg.append("\n"));
    }

    std::cout << logMsg.toStdString();
    
    // Open file if it's not or if it has been deleted, change file if the day has changed
    if(!logFile.isOpen() || currentDate != QDate::currentDate() || !logFile.exists()) {
        if(logFile.isOpen()) {
            logFile.close();
        }
        currentDate = QDate::currentDate();

        QString logLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).append("/logs/");
        if(!QDir(logLocation).exists()) {
            QDir().mkpath(logLocation);
        }
        logFile.setFileName(logLocation + currentDate.toString("yyyy-MM-dd").append(".log"));
        logFile.open(QIODevice::Append);
    }
    if(logFile.write(logMsg.toUtf8()) == -1) {
        std::cout << "Cannot write to " << logFile.fileName().toStdString() << ": " << logFile.errorString().toStdString() << std::endl;
    } else {
        if(!logFile.flush()) 
            std::cout << "Cannot write to " << logFile.fileName().toStdString() << ": " << logFile.errorString().toStdString() << std::endl;
    }

}

