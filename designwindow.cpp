#include "designwindow.hpp"
#include "ui/ui_designwindow.h"

#include "projectManager/sourceeda.hpp"


DesignWindow::DesignWindow(const QString& lib, const QString& cell, const QString& cellview, SourceEDA *parent)
    : QMainWindow((QWidget *)parent)
    , ui(new Ui::DesignWindow), parent(parent)
{
    ui->setupUi(this);
    this->setWindowTitle("Source EDA - " + parent->getProjectName() + " - " + cellview);
}

DesignWindow::~DesignWindow()
{
    delete ui;
}
