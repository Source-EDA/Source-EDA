#include "createcellview.hpp"
#include "../librarymanager.hpp"
#include "log.hpp"

CreateCellview::CreateCellview(SourceEDA *parent, Qt::WindowFlags f) : QDialog(parent, f) {
    seda = parent;
    uiCellviewPopup = new Ui::CreateCellviewPopup;

    uiCellviewPopup->setupUi(this);
    connect(uiCellviewPopup->buttonBox, &QDialogButtonBox::accepted, this, &CreateCellview::createCellview);
    connect(uiCellviewPopup->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::close);
}

CreateCellview::~CreateCellview() {
    delete uiCellviewPopup;
}


void CreateCellview::openCreateCellviewPopup(const QString &for_lib, const QString &for_cell, const QString &cv_type) {
    if(this->isVisible()) {
        this->raise();
    } else {
        this->current_lib = for_lib;
        this->current_cell = for_cell;

        uiCellviewPopup->libCombo->clear();
        LibraryManager * libManager = seda->getLibraryManager(); 
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
        this->show();
    }
}
void CreateCellview::createCellview(void) {
    LibManagerError errCode = seda->getLibraryManager()->createCellview(uiCellviewPopup->libCombo->currentText(),
                                                    uiCellviewPopup->cellCombo->currentText(),
                                                    uiCellviewPopup->typeCombo->currentText(),
                                                    uiCellviewPopup->nameEdit->text());
    if(!errCode) {
        seda->clearCellviews(true);
        seda->showCellview(this->current_lib, this->current_cell);

        // seda->addCellview(uiCellviewPopup->libCombo->currentText(), 
        //                   uiCellviewPopup->cellCombo->currentText(),
        //                   uiCellviewPopup->typeCombo->currentText(),
        //                   uiCellviewPopup->nameEdit->text());
        this->close();
    } else {
        uiCellviewPopup->errorLabel->setText(tr("Error")); // TODO: better error
    }
}


void CreateCellview::updateCellviewCells(const QString &lib_name) {
    uiCellviewPopup->cellCombo->clear();
    for(QString cellname : seda->getLibraryManager()->getCellsFromLib(lib_name)) {
        uiCellviewPopup->cellCombo->addItem(cellname);
    }
}