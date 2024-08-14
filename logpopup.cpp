#include "logpopup.hpp"
#include "ui/ui_logpopup.h"

LogPopup::LogPopup(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LogPopup)
{
    ui->setupUi(this);
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
