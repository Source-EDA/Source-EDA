
#ifndef CREATECELLVIEW_H
#define CREATECELLVIEW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtWidgets/QToolButton>
#include <QFile>
#include <QFileDialog>
#include <QListWidgetItem>

#include "../../ui/ui_create_cellview_popup.h"

class CreateCellview;

#include "../sourceeda.hpp"



class CreateCellview : public QDialog
{
    Q_OBJECT

public:
    CreateCellview(SourceEDA *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~CreateCellview();

public slots:
    void createCellview(void);
    void openCreateCellviewPopup(const QString &for_lib = "", const QString &for_cell = "", const QString &cv_type = "");

private:
    // graphical functions
    void setupMenusActions(void);
    void setupMenus(void);
    QString current_lib;
    QString current_cell;

    Ui::CreateCellviewPopup *uiCellviewPopup;

    void updateCellviewCells(const QString &lib_name);


    SourceEDA *seda;


};

#endif // CREATECELLVIEW_H
