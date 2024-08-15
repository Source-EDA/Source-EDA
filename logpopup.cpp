#include "logpopup.hpp"
#include "log.hpp"
#include "ui/ui_logpopup.h"
#include <QTimer>

LogPopup::LogPopup(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LogPopup)
{
    ui->setupUi(this);
    QObject::connect(ui->close, &QPushButton::clicked, [this] {Log::closePopup(this); } );

    QTimer::singleShot(5000, this, [this] {Log::closePopup(this); });
}

LogPopup::~LogPopup()
{
    delete ui;
}

void LogPopup::setText(QString text) {
    ui->text->setText(text);
}
void LogPopup::setTitle(QString title) {
    ui->title->setText(title);
}

void LogPopup::setLevel(QtMsgType level) {}
