
#ifndef CREATECELL_H
#define CREATECELL_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtWidgets/QToolButton>
#include <QFile>
#include <QFileDialog>
#include <QListWidgetItem>

class CreateCell;

#include "../../ui/ui_create_cell_popup.h"
#include "../sourceeda.hpp"



class CreateCell : public QDialog
{
    Q_OBJECT

public:
    CreateCell(SourceEDA *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~CreateCell();

public slots:

    void openCreateCellPopup(const QString &for_lib = "");
    // void openCreateCellPopupNoLib(void);
    void createCell(void);

private:    
    // graphical functions
    void setupMenusActions(void);
    void setupMenus(void);
    Ui::CreateCellPopup *uiCellPopup;
    QString current_lib;

    SourceEDA *seda;


};

#endif // CREATECELL_H
