#ifndef LOGPOPUP_H
#define LOGPOPUP_H

#include <QDialog>
#include <QtLogging>

namespace Ui {
class LogPopup;
}

class LogPopup : public QDialog
{
    Q_OBJECT

public:
    explicit LogPopup(QWidget *parent = nullptr);
    ~LogPopup();
    void setText(QString text);
    void setTitle(QString title);
    void setLevel(QtMsgType level);

private:
    Ui::LogPopup *ui;
};

#endif // LOGPOPUP_H
