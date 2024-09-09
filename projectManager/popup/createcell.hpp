
#ifndef CREATECELL_H
#define CREATECELL_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtWidgets/QToolButton>
#include <QFile>
#include <QFileDialog>
#include <QListWidgetItem>

#include "../../ui/ui_create_lib_popup.h"
#include "../sourceeda.hpp"



class CreateCell : public QDialog
{
    Q_OBJECT

public:
    CreateCell(SourceEDA *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~CreateCell();

    void openCreateCellPopup(QListWidgetItem *for_lib = NULL);
    void openCreateCellPopupNoLib(void);
    void createCell(void);

private:    
    // graphical functions
    void setupMenusActions(void);
    void setupMenus(void);
    Ui::CreateCellPopup *uiCellPopup;
    QListWidgetItem *current_lib;

    SourceEDA *seda;


};

#endif // CREATECELL_H
