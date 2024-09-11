
#ifndef CREATELIB_H
#define CREATELIB_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtWidgets/QToolButton>
#include <QFile>
#include <QFileDialog>
#include <QListWidgetItem>

#include "../../ui/ui_create_lib_popup.h"
#include "../sourceeda.hpp"



class CreateLib : public QWidget
{
    Q_OBJECT

public:
    CreateLib(SourceEDA *parent = nullptr);
    ~CreateLib();

public slots:
    void openCreateLibPopup(void);
    void createLib(void);

private:    
    // graphical functions
    void setupMenusActions(void);
    void setupMenus(void);
    Ui::CreateLibPopup *uiLibPopup;
    // QWidget *createLibPopup;

    SourceEDA *seda;


};

#endif // CREATELIB_H
