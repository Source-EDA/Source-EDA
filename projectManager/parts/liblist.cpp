#include "liblist.hpp"
#include "../sourceeda.hpp"
#include <QMenu>
#include "../librarymanager.hpp"

LibList::LibList(QWidget* parent) : QListWidget(parent) { 
    // seda = parent;
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(unfoldLib(QListWidgetItem*)));
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(libListContextMenu(QPoint)));

}

void LibList::setSEDA(SourceEDA * s, CreateLib * createLib) {
    seda = s;
    createLibPopup = createLib;
    // createLibPopup = new CreateLib(seda);
}

LibList::~LibList() {
}

// CREATE LIB POPUP
void LibList::libListContextMenu(const QPoint &pos) {
    QMenu contextMenu(tr("Context menu"), this);

    QAction create_lib_action("Create library", this); // TODO: here could use pointer to globalise ? useless ? take more memory for nothing ?
    connect(&create_lib_action, &QAction::triggered, createLibPopup, &CreateLib::openCreateLibPopup);

    QAction rename_lib_action("Rename library", this); // TODO: here could use pointer to globalise ? useless ? take more memory for nothing ?
    QAction delete_lib_action("Delete library", this); // TODO: here could use pointer to globalise ? useless ? take more memory for nothing ?

    QListWidgetItem *item = this->itemAt(pos);
    if(item != nullptr) {
        // TODO: connect actions to a slot
        contextMenu.addAction(&rename_lib_action);
        contextMenu.addAction(&delete_lib_action);
        contextMenu.addSeparator();
    }

    contextMenu.addSeparator();

    contextMenu.addAction(&create_lib_action);

    contextMenu.exec(this->mapToGlobal(pos));
}


void LibList::unfoldLib(QListWidgetItem *lib_item) {
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    lib_item->setSelected(true);
    this->setSelectionMode(QAbstractItemView::NoSelection); // TODO: awful ? betterway ? cancel single click selection

    seda->clearCells(true);
    seda->clearCellviews(false);
    QString libName = lib_item->text();
    seda->showCell(libName);

}
