#include "notificationpopup.hpp"
#include "notification.hpp"
#include "ui/ui_notificationpopup.h"
#include <QTimer>

NotificationPopup::NotificationPopup(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NotificationPopup)
{
    ui->setupUi(this);
    QObject::connect(ui->close, &QPushButton::clicked, [this] {Notification::closeNotification(this); } );

    QTimer::singleShot(5000, this, [this] {Notification::closeNotification(this); });
}

NotificationPopup::~NotificationPopup()
{
    delete ui;
}

void NotificationPopup::setText(QString text) {
    ui->text->setText(text);
}
void NotificationPopup::setTitle(QString title) {
    ui->title->setText(title);
}

void NotificationPopup::setLevel(QtMsgType level) {}
