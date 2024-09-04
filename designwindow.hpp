#ifndef DESIGNWINDOW_H
#define DESIGNWINDOW_H

#include <QMainWindow>

namespace Ui {
class DesignWindow;
}


class SourceEDA;
class DesignWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DesignWindow(const QString& lib, const QString& cell, const QString& cellview, SourceEDA *parent = nullptr);
    ~DesignWindow();

private:
    Ui::DesignWindow *ui;
    SourceEDA *parent;
};

#endif // DESIGNWINDOW_H
