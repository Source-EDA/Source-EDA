#include "cellviewlist.hpp"
#include "../sourceeda.hpp"
// #include <QMenu>
// #include "../librarymanager.hpp"

CellviewList::CellviewList(QWidget* parent) : QListWidget(parent) { 

    // connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(unfoldCell(QListWidgetItem*)));
    // this->setContextMenuPolicy(Qt::CustomContextMenu);
    // connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(cellListContextMenu(QPoint)));
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(cellviewListContextMenu(QPoint)));
    currentLib = "";
    currentCell = "";

}

void CellviewList::setSEDA(SourceEDA * s, CreateCellview *createCellview) {
    seda = s;
    createCellviewPopup = createCellview;
}

CellviewList::~CellviewList() {
}

void CellviewList::showCellview(const QString & fromLibrary, const QString &fromCell) {

    LibraryManager *libManager = seda->getLibraryManager();
    currentLib = fromLibrary;
    currentCell = fromCell;

    map<QString, QString> cv_infos; // TODO: const Qstring ? const &Qstring ? what is better ?

    int8_t return_status = libManager->getCellviewsNameTypeFromCell(fromLibrary, fromCell, &cv_infos);

    for (std::map<QString, QString>::const_iterator it = cv_infos.begin(); it != cv_infos.end(); ++it) {
        QString cvIconName = "cellview";
        if(LibraryManager::getCellviewTypes().contains(it->second)) {
            cvIconName = it->second;
        }

        this->addItem( new QListWidgetItem(QIcon::fromTheme(cvIconName), it->first, 0) );
    }
}




void CellviewList::cellviewListContextMenu(const QPoint &pos) {
    QMenu contextMenu(tr("Context menu"), this);

    QAction create_cellview_action("Create cellview here", this); // TODO: here could use pointer to globalise ? useless ? take more memory for nothing ?
    connect(&create_cellview_action, &QAction::triggered, createCellviewPopup,
            [=]() { createCellviewPopup->openCreateCellviewPopup(currentLib, currentCell); }); // lambda to pass a parameter to slot

    QAction rename_cellview_action("Rename cellview", this); // TODO: here could use pointer to globalise ? useless ? take more memory for nothing ?
    QAction delete_cellview_action("Delete cellview", this); // TODO: here could use pointer to globalise ? useless ? take more memory for nothing ?
    // TODO: connect actions to a slot

    QListWidgetItem *item = this->itemAt(pos);
    if(item != nullptr) {
        contextMenu.addAction(&rename_cellview_action);
        contextMenu.addAction(&delete_cellview_action);

        contextMenu.addSeparator();
    }

    contextMenu.addSeparator();

    contextMenu.addAction(&create_cellview_action);

    contextMenu.exec(this->mapToGlobal(pos));
}



