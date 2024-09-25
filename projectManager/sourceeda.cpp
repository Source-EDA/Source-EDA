#include "sourceeda.hpp"
#include <QSettings>
#include <QStandardPaths>
#include "../notification.hpp"
#include "../log.hpp"
#include "../popup.hpp"
#include <QtLogging>

#include "parts/liblist.hpp"
#include "../ui/ui_mumwindow.h"

SourceEDA::SourceEDA(QWidget *parent)
    : QMainWindow(parent)
{
    setupVariables();

    // style

    ui->setupUi(this);
    ui->lib_list->setSEDA(this, createLibPopup);
    ui->cell_list->setSEDA(this, createCellPopup);
    ui->cellview_list->setSEDA(this, createCellviewPopup);

    Notification::setUp(this);
    Log::setup(ui->logTextZone);
    Popup::setup(this);

    setupMenusActions();
    setupMenus();

    QIcon::setThemeSearchPaths({":/resources/icons"});
    QIcon::setThemeName("default");
    
    QCoreApplication::setOrganizationName("SourceEDA");
    QCoreApplication::setOrganizationDomain("source-eda.com");
    QCoreApplication::setApplicationName("SourceEDA");

    setWindowTitle(tr("Source EDA"));
    setUnifiedTitleAndToolBarOnMac(true);
}

SourceEDA::~SourceEDA()
{
    delete ui;
    delete uiRenameLibPopup;

    // TODO: delete ui for each design windows
    // TODO : probably more delete -> check with valgrind
}

LibraryManager * SourceEDA::getLibraryManager() {
    return this->libManager;
}

void SourceEDA::setupVariables(void) {
    ui = new Ui::SourceEDAWindow;

    createLibPopup = new CreateLib(this);
    createCellPopup = new CreateCell(this, Qt::Window);
    createCellviewPopup = new CreateCellview(this, Qt::Window);
    // uiCellviewPopup = new Ui::CreateCellviewPopup;
    // createCellviewPopup = new QDialog(this, Qt::Window);
    uiRenameLibPopup = new Ui::RenameLibPopup;
    renameLibPopup = new QDialog(this, Qt::Window);

    project_file = nullptr;
    project_path = "";
    project_opened = false;
}


void SourceEDA::setupMenusActions(void)
{
    // File Menu
    menuActionOpen = new QAction(tr("&Open"), this);
    menuActionOpen->setShortcut(tr("Ctrl+O"));
    menuActionOpen->setStatusTip(tr("Opens a project"));
    connect(menuActionOpen, &QAction::triggered, this, &SourceEDA::openProjectPopup);


    //Toolbar
    menuActionNewLib = new QAction(QIcon::fromTheme("create_lib"), tr("&New Library"), this);
    menuActionNewLib->setEnabled(false);
    connect(menuActionNewLib,SIGNAL(triggered()), createLibPopup, SLOT(openCreateLibPopup()));

    menuActionNewCell = new QAction(QIcon::fromTheme("create_cell"), tr("&New Cell"), this);
    menuActionNewCell->setEnabled(false);
    connect(menuActionNewCell, SIGNAL(triggered()), createCellPopup, SLOT(openCreateCellPopup()));

    //menuActionNewCellview = new QAction(QIcon::fromTheme("create_cellview"), tr("&New Cellview"), this);
    menuActionNewCellview = new QToolButton(this);
    menuActionNewCellview->setIcon(QIcon::fromTheme("create_cellview"));
    menuActionNewCellview->setPopupMode(QToolButton::MenuButtonPopup);
    menuActionNewCellview->setEnabled(false);
    QMenu popupMenu(menuActionNewCellview);
    popupMenu.addAction(tr("test"));
    menuActionNewCellview->setMenu(&popupMenu);
    connect(menuActionNewCellview, SIGNAL(clicked()),createCellviewPopup, SLOT(openCreateCellviewPopup()));
    // TODO: open menu with right click ? -> need to override QPushButton::mousePressEvent()

    uiRenameLibPopup->setupUi(renameLibPopup);
    connect(uiRenameLibPopup->buttonBox, &QDialogButtonBox::accepted, this, &SourceEDA::renameLib);
    connect(uiRenameLibPopup->buttonBox, &QDialogButtonBox::rejected, renameLibPopup, &QWidget::close);



    connect(ui->cellview_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(openCellview(QListWidgetItem*)));

    // uiCellviewPopup->setupUi(createCellviewPopup);
    // connect(uiCellviewPopup->libCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(updateCellviewCells(QString)));
    // connect(uiCellviewPopup->buttonBox, &QDialogButtonBox::accepted, this, &SourceEDA::createCellview);
    // connect(uiCellviewPopup->buttonBox, &QDialogButtonBox::rejected, createCellviewPopup, &QDialog::close);

    //TODO: add create lib from lib list right click
    // TODO: add create cell from cell list right click -> auto select current lib
}

void SourceEDA::setupMenus(void)
{
    // Top Menu
    menuFile = ui->menubar->addMenu(tr("&File"));
    menuFile->addAction(menuActionOpen);
    menuFile->addAction(menuActionNewLib);
    menuFile->addSeparator();
    menuFile->addAction(tr("test")); // TODO: remove

    // Toolbar
    ui->toolBar->addAction(menuActionNewLib);
    ui->toolBar->addAction(menuActionNewCell);
    ui->toolBar->addWidget(menuActionNewCellview);

    // TODO: decide if for cellview we use a dropdown menu
    /*ui->toolBar->addSeparator();

    QToolButton *popupButton = new QToolButton(ui->toolBar);
    popupButton->setIcon(QIcon::fromTheme("create_cellview"));
    popupButton->setPopupMode(QToolButton::MenuButtonPopup);
    QMenu *popupMenu = new QMenu(popupButton);
    popupMenu->addAction(tr("test"));
    popupButton->setMenu(popupMenu);
    ui->toolBar->addWidget(popupButton);*/
}

void SourceEDA::openProjectPopup(void)
{
    QSettings settings;
    QFileDialog openProjectDialog(this);
    openProjectDialog.setFileMode(QFileDialog::ExistingFile);
    openProjectDialog.setNameFilter("*.pro");
    openProjectDialog.setViewMode(QFileDialog::Detail);

    QString defaultOpenPath = settings.value("lib/defaultOpenProjectPath", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();
    openProjectDialog.setDirectory(defaultOpenPath);

    if(openProjectDialog.exec()) {
        QStringList project_files = openProjectDialog.selectedFiles();
        project_file = new QFile( project_files[0] );
        settings.setValue("lib/defaultOpenProjectPath", QFileInfo(*project_file).absolutePath());
        project_file->open( QFile::ReadOnly );
        try {
            project_data = json::parse( QString(project_file->readAll()).toStdU32String() );
        } catch (const json::parse_error& e) {
            Popup::popup(QtCriticalMsg, tr("Project Manager"), tr("Project file parse error"), tr("Could not parse the given project file. See logs for more information."));
            Log::write(QtCriticalMsg, tr("Project Manager"), tr("Project file parse error"), e.what());
        }
        project_file->close();

        if(!project_data.is_null()) {
            if(!checkProjectJSON()) {
                QStringList tmp_str_list = project_files[0].split("/");
                tmp_str_list.removeLast();
                project_path = tmp_str_list.join("/") + "/";

                setupProject();
            } else {
                project_path = "";
                Popup::popup(QtCriticalMsg, tr("Project Manager"), tr("Bad project file"), tr("Some data were missing or unexpected in project file. See logs for more information"));
            }
        }
    }
}

project_parse_error SourceEDA::checkProjectJSON(void) {
    //TODO: log error information in internal debug/log
    if(!project_data.contains("project_name")) {
        // qCritical("(Parse Project File) Project Name is missing.");
        Log::write(QtCriticalMsg, tr("Project Manager"), tr("Bad project file"), tr("Project Name is missing.")); 
        return PRO_PARSE_MISSING_DATA;
    }
    if(project_data["project_name"] == "") {
        // qCritical("(Parse Project File) Project Name shouldn't be empty.");
        Log::write(QtCriticalMsg, tr("Project Manager"), tr("Bad project file"), tr("Project Name shouldn't be empty.")); 

        return PRO_PARSE_MISSING_DATA;
    }
    if(!project_data.contains("source_eda_version")) {
        Log::write(QtCriticalMsg, tr("Project Manager"), tr("Bad project file"), tr("Project's Source EDA version is missing."));
        return PRO_PARSE_MISSING_DATA;
    }
    if(!project_data["source_eda_version"].is_number_float()) {
        Log::write(QtCriticalMsg, tr("Project Manager"), tr("Bad project file"), tr("Project's Source EDA version should be a float."));
        return PRO_PARSE_WRONG_FORMAT;
    }
    if(project_data["source_eda_version"] > SEDA_VERSION) {
        Log::write(QtCriticalMsg, tr("Project Manager"), tr("Bad project file"), "Project's Source EDA version is " + QString::number((float) project_data["source_eda_version"]) + ", whereas Source EDA is only " + QString::number(SEDA_VERSION) + ". Please update Source EDA.");
        return PRO_PARSE_WRONG_VERSION;
    }

    return PRO_PARSE_SUCCESS;
}


void SourceEDA::setupProject(void) {
    this->setWindowTitle("Source EDA - " + QString::fromStdString(project_data["project_name"]));

    libManager = new LibraryManager(this);

    reloadLibraries();

    menuActionNewLib->setEnabled(true);
    menuActionNewCell->setEnabled(true);
    menuActionNewCellview->setEnabled(true);
    this->ui->lib_list->setEnabled(true);
    this->ui->lib_list->setContextMenuPolicy(Qt::CustomContextMenu);

    project_opened = true;
}


void SourceEDA::reloadLibraries(void) {
    json* lib_db = libManager->getDb();

    ui->lib_list->clear();

    for(json lib : *lib_db) {
        ui->lib_list->addItem( new QListWidgetItem(QIcon::fromTheme("library"), QString::fromStdString(lib["lib_name"].get<string>()), 0) );
    }
}




void SourceEDA::openRenameLibPopup(const QString &oldName)
{
    // if(renameLibPopup->isVisible()) {
    //     renameLibPopup->raise();
    // } else {
    //     uiLibPopup->libNameEdit->setText(oldName);
    //     uiLibPopup->errorLabel->setText("");
    //     renameLibPopup->show();
    // }
}


void SourceEDA::renameLib(void) {

    // LibManagerError errCode = libManager->renameLib("", uiLibPopup->libNameEdit->text());

    // if(!errCode) {
    //     ui->lib_list->addItem( new QListWidgetItem(QIcon::fromTheme("library"), uiLibPopup->libNameEdit->text(), 0) );
    //     createLibPopup->close();
    // } else {
    //     uiLibPopup->errorLabel->setText(tr("Error")); // TODO: better error
    // }
}



void SourceEDA::openCellview(QListWidgetItem *cellview_item) {
    DesignWindow *new_window = new DesignWindow(ui->lib_list->currentItem()->text(), ui->cell_list->currentItem()->text(),
                                                cellview_item->text(), this);

    this->design_windows.push_back(new_window);
    //TODO: remove from list when window is closed

    new_window->show();
}

// GETTERS / SETTERS
// getter / setters
QString SourceEDA::getProjectPath(void) const {
    return this->project_path;
}
QString SourceEDA::getProjectName(void) const {
    return QString::fromStdString(project_data["project_name"]);
}

// EVENT
void SourceEDA::closeEvent(QCloseEvent *event)
{
    qInfo() << "Exiting Source EDA"; // TODO: Remove
    // TODO: Manage / check / save stuff
    QMainWindow::closeEvent(event);
}


void SourceEDA::resizeEvent(QResizeEvent *event)
{
    Notification::updatePosition();
    QWidget::resizeEvent(event);
}

void SourceEDA::addCellview(const QString & libName, const QString & cellName, const QString & typeName, const QString &name) {
    if(ui->lib_list->currentItem()->text() == libName &&
        ui->cell_list->currentItem()->text() == cellName)
    {
        ui->cellview_list->addItem( new QListWidgetItem(QIcon::fromTheme(typeName),
                                                   name, 0) );
    }
}

void SourceEDA::clearCellviews(bool enabled) {
    ui->cellview_list->setEnabled(enabled);
    ui->cellview_list->clear();
}

void SourceEDA::clearCells(bool enabled) {
    ui->cell_list->setEnabled(enabled);
    ui->cell_list->clear();
}

void SourceEDA::showCell(const QString &libName) {
    ui->cell_list->showCell(libName);
}


void SourceEDA::showCellview(const QString &libName, const QString &cellName) {
    ui->cellview_list->showCellview(libName, cellName);
}