#include "log.hpp"
#include <QString>
#include <QMessageBox>
#include "logpopup.hpp"
#include <iostream>

QtMessageHandler Log::originalHandler = nullptr;
QMainWindow * Log::main = nullptr;

void Log::log(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{

    // TODO Cleanup
    std::cout << msg.toStdString() << std::endl;
    LogPopup msgPopup(main);
    QString message_icon_name;
    switch (type) {
        case QtDebugMsg:
            message_icon_name = "warning";
            break;
        case QtInfoMsg:
            message_icon_name = "error";
            break;
        case QtWarningMsg:
        default:
            message_icon_name = "info";
            break;
    }
    // msgPopup.setIconPixmap(QIcon::fromTheme(message_icon_name).pixmap(32));
    msgPopup.setWindowTitle(message_icon_name);
    msgPopup.setText(msg);
    msgPopup.setTitle(message_icon_name);
    // msgPopup.setBaseSize(100, 300);
    // msgPopup.setVisible(true);
    msgPopup.setObjectName("log");
    // msgPopup.setInformativeText(msg);
    // msgPopup.setStandardButtons(QMessageBox::Ok);
    msgPopup.setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    msgPopup.exec();


    // if (originalHandler)
        // *originalHandler(type, context, msg);
}

void Log::setUp(QMainWindow * mainWin) {
    originalHandler = qInstallMessageHandler(log);
    main = mainWin;
}