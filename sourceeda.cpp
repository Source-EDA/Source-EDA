#include "sourceeda.hpp"
#include <QSettings>
#include <QStandardPaths>
#include "notification.hpp"
#include "log.hpp"
#include "popup.hpp"
#include <QtLogging>

SourceEDA::SourceEDA(QWidget *parent)
    : QMainWindow(parent)
{
    setupVariables();

    // style
    QFile styleFile( ":/resources/stylesheet.qss" );
    styleFile.open( QFile::ReadOnly );
    stylesheetContent = QString(styleFile.readAll());
    this->setStyleSheet( stylesheetContent );
    createLibPopup->setStyleSheet(stylesheetContent);

    ui->setupUi(this);

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
    delete uiLibPopup;
    delete uiCellPopup;
    delete uiCellviewPopup;
}


void SourceEDA::setupVariables(void) {
    ui = new Ui::SourceEDAWindow;

    uiLibPopup = new Ui::CreateLibPopup;
    createLibPopup = new QWidget(this, Qt::Window);
    uiCellPopup = new Ui::CreateCellPopup;
    createCellPopup = new QDialog(this, Qt::Window);
    uiCellviewPopup = new Ui::CreateCellviewPopup;
    createCellviewPopup = new QDialog(this, Qt::Window);

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
    connect(menuActionNewLib, &QAction::triggered, this, &SourceEDA::openCreateLibPopup);

    menuActionNewCell = new QAction(QIcon::fromTheme("create_cell"), tr("&New Cell"), this);
    menuActionNewCell->setEnabled(false);
    connect(menuActionNewCell, SIGNAL(triggered()), this, SLOT(openCreateCellPopup()));

    //menuActionNewCellview = new QAction(QIcon::fromTheme("create_cellview"), tr("&New Cellview"), this);
    menuActionNewCellview = new QToolButton(this);
    menuActionNewCellview->setIcon(QIcon::fromTheme("create_cellview"));
    menuActionNewCellview->setPopupMode(QToolButton::MenuButtonPopup);
    menuActionNewCellview->setEnabled(false);
    QMenu popupMenu(menuActionNewCellview);
    popupMenu.addAction(tr("test"));
    menuActionNewCellview->setMenu(&popupMenu);
    connect(menuActionNewCellview, SIGNAL(clicked()), this, SLOT(openCreateCellviewPopup()));
    // TODO: open menu with right click ? -> need to override QPushButton::mousePressEvent()

    uiLibPopup->setupUi(createLibPopup);
    connect(uiLibPopup->buttonBox, &QDialogButtonBox::accepted, this, &SourceEDA::createLib);
    connect(uiLibPopup->buttonBox, &QDialogButtonBox::rejected, createLibPopup, &QWidget::close);

    uiCellPopup->setupUi(createCellPopup);
    connect(uiCellPopup->buttonBox, &QDialogButtonBox::accepted, this, &SourceEDA::createCell);
    connect(uiCellPopup->buttonBox, &QDialogButtonBox::rejected, createCellPopup, &QDialog::close);

    connect(ui->lib_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(unfoldLib(QListWidgetItem*)));
    connect(ui->cell_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(unfoldCell(QListWidgetItem*)));
    ui->cell_list->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->cell_list, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(cellListContextMenu(QPoint)));

    uiCellviewPopup->setupUi(createCellviewPopup);
    connect(uiCellviewPopup->libCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(updateCellviewCells(QString)));
    connect(uiCellviewPopup->buttonBox, &QDialogButtonBox::accepted, this, &SourceEDA::createCellview);
    connect(uiCellviewPopup->buttonBox, &QDialogButtonBox::rejected, createCellviewPopup, &QDialog::close);

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

    ui->toolBar->addSeparator();
    QToolButton *popupButton = new QToolButton(ui->toolBar);
    popupButton->setIcon(QIcon::fromTheme("create_cellview"));
    popupButton->setPopupMode(QToolButton::MenuButtonPopup);
    QMenu *popupMenu = new QMenu(popupButton);
    popupMenu->addAction(tr("test"));
    popupButton->setMenu(popupMenu);
    ui->toolBar->addWidget(popupButton);
}

// void SourceEDA::throwMsgPopup(seda_msg_type msg_type, const QString &title, const QString &message) {
//     QMessageBox msgPopup(this);
//     QString message_icon_name;
//     switch (msg_type) {
//         case MSG_WARNING:
//             message_icon_name = "warning";
//             break;
//         case MSG_ERROR:
//             message_icon_name = "error";
//             break;
//         case MSG_INFO:
//         default:
//             message_icon_name = "info";
//             break;
//     }
//     msgPopup.setIconPixmap(QIcon::fromTheme(message_icon_name).pixmap(32));
//     msgPopup.setWindowTitle(title);
//     msgPopup.setText(title);
//     msgPopup.setInformativeText(message);
//     msgPopup.setStandardButtons(QMessageBox::Ok);
//     msgPopup.exec();
// }


void SourceEDA::openProjectPopup(void)
{
    QSettings settings;
    QFileDialog openProjectDialog(this);
    openProjectDialog.setFileMode(QFileDialog::ExistingFile);
    openProjectDialog.setNameFilter("*.pro");
    openProjectDialog.setViewMode(QFileDialog::Detail);

    QString defaultOpenPath = settings.value("lib/defaultOpenProjectPath").toString();
    openProjectDialog.setDirectory(defaultOpenPath == "" ? QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) : defaultOpenPath);

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

    populateLibraries();

    menuActionNewLib->setEnabled(true);
    menuActionNewCell->setEnabled(true);
    menuActionNewCellview->setEnabled(true);
    this->ui->lib_list->setEnabled(true);
    this->ui->lib_list->setContextMenuPolicy(Qt::CustomContextMenu);

    project_opened = true;
}

void SourceEDA::populateLibraries(void) {
    json* lib_db = libManager->getDb();

    for(json lib : *lib_db) {
        ui->lib_list->addItem( new QListWidgetItem(QIcon::fromTheme("library"), QString::fromStdString(lib["lib_name"].get<string>()), 0) );
    }
}


// CREATE LIB POPUP
void SourceEDA::openCreateLibPopup(void)
{
    if(createLibPopup->isVisible()) {
        createLibPopup->raise();
    } else {
        uiLibPopup->libNameEdit->setText("");
        uiLibPopup->errorLabel->setText("");
        createLibPopup->show();
    }
}
void SourceEDA::createLib(void) {
    LibManagerError errCode = libManager->createLib(uiLibPopup->libNameEdit->text());

    if(!errCode) {
        ui->lib_list->addItem( new QListWidgetItem(QIcon::fromTheme("library"), uiLibPopup->libNameEdit->text(), 0) );
        createLibPopup->close();
    } else {
        uiLibPopup->errorLabel->setText(tr("Error")); // TODO: better error
    }
}
void SourceEDA::cellListContextMenu(const QPoint &pos) {
    QMenu contextMenu(tr("Context menu"), this);

    QAction create_cell_action("Create cell here", this); //TODO: generalyse with class pointer ?
    connect(&create_cell_action, &QAction::triggered, this,
            [=]() { openCreateCellPopup(ui->lib_list->currentItem()->text()); }); // lambda to pass a parameter to slot
    contextMenu.addAction(&create_cell_action);

    contextMenu.exec(ui->cell_list->mapToGlobal(pos));
}
void SourceEDA::openCreateCellPopup(const QString &for_lib) {
    if(createCellPopup->isVisible()) {
        createCellPopup->raise();
    } else {
        uiCellPopup->cellLibCombo->clear();
        for(json lib : *libManager->getDb()) {
            uiCellPopup->cellLibCombo->addItem(QString::fromStdString(lib["lib_name"].get<string>()));
        }

        if(for_lib != "") {
            uiCellPopup->cellLibCombo->setCurrentText(for_lib);
        }

        uiCellPopup->cellNameEdit->setText("");
        uiCellPopup->errorLabel->setText("");
        createCellPopup->show();
    }
}
void SourceEDA::createCell(void) {
    LibManagerError errCode = libManager->createCell(uiCellPopup->cellLibCombo->currentText(),
                                                     uiCellPopup->cellNameEdit->text());

    if(!errCode) {
        if(ui->lib_list->currentItem()->text() == uiCellPopup->cellLibCombo->currentText()) {
            ui->cell_list->addItem( new QListWidgetItem(QIcon::fromTheme("cell"), uiCellPopup->cellNameEdit->text(), 0) );
        }
        createCellPopup->close();
    } else {
        uiCellPopup->errorLabel->setText(tr("Error")); // TODO: better error
    }
}
void SourceEDA::openCreateCellviewPopup(const QString &for_lib, const QString &for_cell, const QString &cv_type) {
    if(createCellviewPopup->isVisible()) {
        createCellviewPopup->raise();
    } else {
        uiCellviewPopup->libCombo->clear();
        for(json lib : *libManager->getDb()) {
            uiCellviewPopup->libCombo->addItem(QString::fromStdString(lib["lib_name"].get<string>()));
        }

        uiCellviewPopup->cellCombo->clear();
        for(QString cellname : libManager->getCellsFromLib(uiCellviewPopup->libCombo->currentText())) {
            uiCellviewPopup->cellCombo->addItem(cellname);
        }

        uiCellviewPopup->typeCombo->clear();
        for(QString type : LibraryManager::getCellviewTypes()) {
            uiCellviewPopup->typeCombo->addItem(type);
        }

        if(for_lib != "") {
            uiCellviewPopup->libCombo->setCurrentText(for_lib);
        }
        if(for_cell != "") {
            uiCellviewPopup->cellCombo->setCurrentText(for_cell);
        }
        if(cv_type != "") {
            uiCellviewPopup->typeCombo->setCurrentText(cv_type);
        }

        uiCellviewPopup->nameEdit->setText("");
        uiCellviewPopup->errorLabel->setText("");
        createCellviewPopup->show();
    }
}
void SourceEDA::createCellview(void) {
    LibManagerError errCode = libManager->createCellview(uiCellviewPopup->libCombo->currentText(),
                                                    uiCellviewPopup->cellCombo->currentText(),
                                                    uiCellviewPopup->typeCombo->currentText(),
                                                    uiCellviewPopup->nameEdit->text());

    if(!errCode) {
        if(ui->lib_list->currentItem()->text() == uiCellviewPopup->libCombo->currentText() &&
            ui->cell_list->currentItem()->text() == uiCellviewPopup->cellCombo->currentText())
        {
            ui->cellview_list->addItem( new QListWidgetItem(QIcon::fromTheme(uiCellviewPopup->typeCombo->currentText()),
                                                       uiCellviewPopup->nameEdit->text(), 0) );
        }
        createCellviewPopup->close();
    } else {
        uiCellviewPopup->errorLabel->setText(tr("Error")); // TODO: better error
    }
}

void SourceEDA::updateCellviewCells(const QString &lib_name) {
    uiCellviewPopup->cellCombo->clear();
    for(QString cellname : libManager->getCellsFromLib(lib_name)) {
        uiCellviewPopup->cellCombo->addItem(cellname);
    }
}

void SourceEDA::unfoldLib(QListWidgetItem *lib_item) {
    ui->lib_list->setSelectionMode(QAbstractItemView::SingleSelection);
    lib_item->setSelected(true);
    ui->lib_list->setSelectionMode(QAbstractItemView::NoSelection); // TODO: awful ? betterway ? cancel single click selection

    ui->cellview_list->setEnabled(false);
    ui->cellview_list->clear();

    ui->cell_list->setEnabled(true);
    ui->cell_list->clear();

    for(QString cell_name : libManager->getCellsFromLib(lib_item->text())) {
        ui->cell_list->addItem( new QListWidgetItem(QIcon::fromTheme("cell"), cell_name, 0) );
    }
}
void SourceEDA::unfoldCell(QListWidgetItem *cell_item) {
    ui->cell_list->setSelectionMode(QAbstractItemView::SingleSelection);
    cell_item->setSelected(true);
    ui->cell_list->setSelectionMode(QAbstractItemView::NoSelection); // TODO: awful ? betterway ? cancel single click selection

    ui->cellview_list->setEnabled(true);
    ui->cellview_list->clear();

    for(QString cellview_name : libManager->getCellviewsFromCell(ui->lib_list->currentItem()->text(), cell_item->text())) {
        //TODO: add item with icon (depding on type), change getter to send name + type
        ui->cellview_list->addItem(cellview_name);
    }
}


// GETTERS / SETTERS
// getter / setters
QString SourceEDA::getProjectPath(void) const {
    return this->project_path;
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