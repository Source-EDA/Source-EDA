#include "designwindow.hpp"
#include "ui/ui_designwindow.h"

#include "projectManager/sourceeda.hpp"


DesignWindow::DesignWindow(const QString& lib, const QString& cell, const QString& cellview, SourceEDA *parent)
    : QMainWindow((QWidget *)parent)
    , ui(new Ui::DesignWindow), parent(parent)
{
    ui->setupUi(this);
    this->setWindowTitle("Source EDA - " + parent->getProjectName() + " - " + cellview);

    this->scene = new QGraphicsScene();
    ui->graphicsView->setScene(this->scene);

    //this->scene->setBackgroundBrush(QColor(255, 255, 255));
    //this->scene->setForegroundBrush(QBrush(Qt::black, Qt::Dense1Pattern));

    this->scene->addRect(QRectF(QPointF(0,0), QPointF(100, 65)), QPen(QColor(200, 150, 120)), QBrush(Qt::green, Qt::CrossPattern));
}

DesignWindow::~DesignWindow()
{
    delete ui;
}
