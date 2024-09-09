#include "createcell.hpp"
#include "../librarymanager.hpp"

CreateCell::CreateCell(SourceEDA *parent, Qt::WindowFlags f) : QDialog(parent, f) {
    seda = parent;
    uiCellPopup = new Ui::CreateCellPopup;

    uiCellPopup->setupUi(this);
    connect(uiCellPopup->buttonBox, &QDialogButtonBox::accepted, this, &CreateCell::createCell);
    connect(uiCellPopup->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::close);
}

CreateCell::~CreateCell() {
    delete uiCellPopup;
}

void CreateCell::openCreateCellPopupNoLib(void) {
    this->openCreateCellPopup(NULL);
}


void CreateCell::openCreateCellPopup(QListWidgetItem *for_lib) {
    if(this->isVisible()) {
        this->raise();
    } else {
        this->current_lib = for_lib;
        uiCellPopup->cellLibCombo->clear();
        LibraryManager * libManager = seda->getLibraryManager();
        for(json lib : *libManager->getDb()) {
            uiCellPopup->cellLibCombo->addItem(QString::fromStdString(lib["lib_name"].get<string>()));
        }

        if(for_lib != NULL) {
            uiCellPopup->cellLibCombo->setCurrentText(for_lib->text());
        }

        uiCellPopup->cellNameEdit->setText("");
        uiCellPopup->errorLabel->setText("");
        this->show();
    }
}

void CreateCell::createCell(void) {
    LibManagerError errCode = seda->getLibraryManager()->createCell(uiCellPopup->cellLibCombo->currentText(),
                                                     uiCellPopup->cellNameEdit->text());

    if(!errCode) {
        if(current_lib != NULL) {
            seda->unfoldLib(current_lib);
        }
        // if(ui->lib_list->currentItem()->text() == uiCellPopup->cellLibCombo->currentText()) {
        //     ui->cell_list->addItem( new QListWidgetItem(QIcon::fromTheme("cell"), uiCellPopup->cellNameEdit->text(), 0) );
        // }
        this->close();
    } else {
        uiCellPopup->errorLabel->setText(tr("Error")); // TODO: better error
    }
}

