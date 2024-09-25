#ifndef CELLVIEWLIST_H
#define CELLVIEWLIST_H

#include <QListWidget>

class SourceEDA;
class CreateCellview;

class CellviewList : public QListWidget
{
    Q_OBJECT

public:
    CellviewList(QWidget* parent);
    ~CellviewList();
    CreateCellview *createCellviewPopup;
    void setSEDA(SourceEDA * s, CreateCellview *createCellview);
    void showCellview(const QString &libName, const QString &fromCell);


private:
    SourceEDA * seda;
    QString currentLib;
    QString currentCell;

private slots:
    // void unfoldCell(QListWidgetItem *cell_item);
    void cellviewListContextMenu(const QPoint &pos);
};

#endif // CELLVIEWLIST_H
