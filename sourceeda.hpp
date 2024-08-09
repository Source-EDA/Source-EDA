#ifndef SOURCEEDA_H
#define SOURCEEDA_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtWidgets/QToolButton>
#include <QFile>
#include <QFileDialog>
#include <QListWidgetItem>

#include "ui/ui_mumwindow.h"
#include "ui/ui_create_lib_popup.h"
#include "ui/ui_create_cell_popup.h"
#include "ui/ui_create_cellview_popup.h"

#include "ext_libs/json.hpp"

#include "librarymanager.hpp"


#define SEDA_VERSION 0.1


using json = nlohmann::json;

enum seda_msg_type {MSG_INFO = 0, MSG_WARNING, MSG_ERROR};
enum project_parse_error {PRO_PARSE_SUCCESS = 0, PRO_PARSE_MISSING_DATA, PRO_PARSE_WRONG_FORMAT, PRO_PARSE_WRONG_VERSION};

QT_BEGIN_NAMESPACE
namespace Ui {
    class SourceEDAWindow;
}
QT_END_NAMESPACE

class SourceEDA : public QMainWindow
{
    Q_OBJECT

public:
    SourceEDA(QWidget *parent = nullptr);
    ~SourceEDA();

    // getter / setters
    QString getProjectPath(void) const;
    void throwMsgPopup(seda_msg_type msg_type, const QString &title, const QString &message);

private:    
    // graphical functions
    void setupVariables(void);
    void setupMenusActions(void);
    void setupMenus(void);
    project_parse_error checkProjectJSON(void);
    void setupProject(void);
    void populateLibraries(void);

    // Libs
    LibraryManager *libManager;

    // Ui
    Ui::SourceEDAWindow *ui;
    Ui::CreateLibPopup *uiLibPopup;
    Ui::CreateCellPopup *uiCellPopup;
    Ui::CreateCellviewPopup *uiCellviewPopup;

    //general variables
    QString stylesheetContent;
    QFile *project_file;
    QString project_path;
    json project_data;
    bool project_opened;


    // Menu bar
    QMenu *menuFile;

    QAction *menuActionOpen;
    QAction *menuActionNewLib;
    QAction *menuActionNewCell;
    QToolButton *menuActionNewCellview;

    // other windows
    QWidget *createLibPopup;
    QDialog *createCellPopup;
    QDialog *createCellviewPopup;

    // EVENT
    void closeEvent(QCloseEvent *event) override;

private slots:
    void openProjectPopup(void);

    void openCreateLibPopup(void);
    void createLib(void);
    void cellListContextMenu(const QPoint &pos);
    void openCreateCellPopup(const QString &for_lib = "");
    void createCell(void);
    void openCreateCellviewPopup(const QString &for_lib = "", const QString &for_cell = "", const QString &cv_type = "");
    void updateCellviewCells(const QString &lib_name);
    void createCellview(void);
    void unfoldLib(QListWidgetItem *lib_item);
    void unfoldCell(QListWidgetItem *cell_item);
};

#endif // SOURCEEDA_H
