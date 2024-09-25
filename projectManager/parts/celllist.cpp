#include "celllist.hpp"
#include "../sourceeda.hpp"
// #include <QMenu>
// #include "../librarymanager.hpp"

CellList::CellList(QWidget* parent) : QListWidget(parent) { 

    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(unfoldCell(QListWidgetItem*)));
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(cellListContextMenu(QPoint)));
    currentLib = "";

}

void CellList::setSEDA(SourceEDA * s, CreateCell * createCell) {
    seda = s;
    createCellPopup = createCell;
}

CellList::~CellList() {
}

void CellList::showCell(const QString & fromLibrary) {

    LibraryManager *libManager = seda->getLibraryManager();
    currentLib = fromLibrary;
    for(QString cell_name : libManager->getCellsFromLib(fromLibrary)) {
        this->addItem( new QListWidgetItem(QIcon::fromTheme("cell"), cell_name, 0) );
    }

}

void CellList::unfoldCell(QListWidgetItem *cell_item) {
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    cell_item->setSelected(true);
    this->setSelectionMode(QAbstractItemView::NoSelection); // TODO: awful ? betterway ? cancel single click selection

    seda->clearCellviews(true);
    seda->showCellview(currentLib, cell_item->text());

    // for(QString cellview_name : seda->getLibraryManager()->getCellviewsFromCell(seda->getCurrentLibName(), cell_item->text())) {
    //     //TODO: add item with icon (depding on type), change getter to send name + type
    //     ui->cellview_list->addItem(cellview_name);
    // }
}

void CellList::cellListContextMenu(const QPoint &pos) {
    QMenu contextMenu(tr("Context menu"), this);

    QAction create_cell_action("Create cell here", this); // TODO: here could use pointer to globalise ? useless ? take more memory for nothing ?
    connect(&create_cell_action, &QAction::triggered, createCellPopup,
            [=]() { createCellPopup->openCreateCellPopup(currentLib); }); // lambda to pass a parameter to slot

    QAction rename_cell_action("Rename cell", this); // TODO: here could use pointer to globalise ? useless ? take more memory for nothing ?
    QAction delete_cell_action("Delete cell", this); // TODO: here could use pointer to globalise ? useless ? take more memory for nothing ?
    // TODO: connect actions to a slot

    QListWidgetItem *item = this->itemAt(pos);
    if(item != nullptr) {
        contextMenu.addAction(&rename_cell_action);
        contextMenu.addAction(&delete_cell_action);

        contextMenu.addSeparator();
    }

    contextMenu.addSeparator();

    contextMenu.addAction(&create_cell_action);

    contextMenu.exec(this->mapToGlobal(pos));
}
