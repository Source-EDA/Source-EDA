#ifndef NOTIFICATIONPOPUP_H
#define NOTIFICATIONPOPUP_H

#include <QDialog>
#include <QtLogging>

namespace Ui {
class NotificationPopup;
}

class NotificationPopup : public QDialog
{
    Q_OBJECT

public:
    explicit NotificationPopup(QWidget *parent = nullptr);
    ~NotificationPopup();
    void setText(QString text);
    void setTitle(QString title);
    void setLevel(QtMsgType level);

private:
    Ui::NotificationPopup *ui;
};

#endif // NOTIFICATIONPOPUP_H
