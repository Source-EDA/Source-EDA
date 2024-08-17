#ifndef POPUP_H
#define POPUP_H

#include <QMessageBox>
#include <QtLogging>
#include <QWidget>



class Popup : public QMessageBox
{
    Q_OBJECT

public:
    // explicit Popup(QWidget *parent = nullptr, QtMsgType level, QString title, QString text);
    static void popup(QtMsgType level, QString context, QString title, QString text = "");
    static void setup(QWidget * mainWin);
private:
    static QWidget * main;
    // ~Popup();
};

#endif // POPUP_H
