#include "createlib.hpp"
#include "../librarymanager.hpp"

CreateLib::CreateLib(SourceEDA * parent) {
    seda = parent;
    uiLibPopup = new Ui::CreateLibPopup;

    uiLibPopup->setupUi(this);
    connect(uiLibPopup->buttonBox, &QDialogButtonBox::accepted, this, &CreateLib::createLib);
    connect(uiLibPopup->buttonBox, &QDialogButtonBox::rejected, createLibPopup, &QWidget::close);

    // createLibPopup = new QWidget(parent, Qt::Window);
}

CreateLib::~CreateLib() {
    delete uiLibPopup;
}


void CreateLib::openCreateLibPopup(void)
{
    if(this->isVisible()) {
        this->raise();
    } else {
        uiLibPopup->libNameEdit->setText("");
        uiLibPopup->errorLabel->setText("");
        this->show();
    }
}


void CreateLib::createLib(void) {
    LibManagerError errCode = seda->getLibraryManager()->createLib(uiLibPopup->libNameEdit->text());

    if(!errCode) {
        // seda->ui->lib_list->addItem( new QListWidgetItem(QIcon::fromTheme("library"), uiLibPopup->libNameEdit->text(), 0) );
        seda->reloadLibraries();
        createLibPopup->close();
    } else {
        uiLibPopup->errorLabel->setText(tr("Error")); // TODO: better error
    }
}
