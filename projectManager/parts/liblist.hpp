#ifndef LIBLIST_H
#define LIBLIST_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtWidgets/QToolButton>
#include <QListWidget>
#include <QMenu>

// #include "../sourceeda.hpp"
// #include "../popup/createlib.hpp"

class SourceEDA;
class CreateLib;

class LibList : public QListWidget
{
    Q_OBJECT

public:
    LibList(QWidget* parent);
    ~LibList();
    CreateLib *createLibPopup;
    void setSEDA(SourceEDA * s, CreateLib * createLib);


private:
    SourceEDA * seda;
private slots:
    void libListContextMenu(const QPoint &pos);
    void unfoldLib(QListWidgetItem *lib_item);

};

#endif // LIBLIST_H
