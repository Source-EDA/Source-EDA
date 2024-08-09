#ifndef LIBRARYMANAGER_H
#define LIBRARYMANAGER_H

#include <QString>
#include <QFile>
#include <QObject>

#include "ext_libs/json.hpp"


using namespace std;
using json = nlohmann::json;

enum LibManagerError { LM_SUCESS = 0, LM_BAD_NAME, LM_BAD_CV_TYPE };

class SourceEDA;
class LibraryManager
{

public:
    static map<QString, QString> cellviewTypesExt;

    LibraryManager(SourceEDA *project);
    LibManagerError createLib(const QString &lib_name);
    LibManagerError createCell(const QString &lib_parent, const QString &cell_name);
    LibManagerError createCellview(const QString &lib_parent, const QString &cell_parent,
                                   const QString &celview_type, const QString &cellview_name);

    //getters / setters
    json* getDb(void);
    QStringList getCellsFromLib(const QString lib_name);
    QStringList getCellviewsFromCell(const QString lib_name, const QString cell_name);
    static QStringList getCellviewTypes(void);

private:
    SourceEDA *project;
    QFile *lib_db_file;
    json lib_db;

};

#endif // LIBRARYMANAGER_H
