#ifndef CELLLIST_H
#define CELLLIST_H

#include <QListWidget>

class SourceEDA;
class CreateCell;

class CellList : public QListWidget
{
    Q_OBJECT

public:
    CellList(QWidget* parent);
    ~CellList();
    void setSEDA(SourceEDA * s, CreateCell * createCell);
    void showCell(const QString &libName);


private:
    CreateCell *createCellPopup;
    SourceEDA * seda;
    QString currentLib;

private slots:
    void unfoldCell(QListWidgetItem *cell_item);
    void cellListContextMenu(const QPoint &pos);
};

#endif // CELLLIST_H
