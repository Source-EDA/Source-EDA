#include "librarymanager.hpp"

#include "sourceeda.hpp"
#include "../popup.hpp"
#include "../log.hpp"
#include <QtLogging>

// TODO: split into smaller methods
LibraryManager::LibraryManager(SourceEDA *project) {
    this->project = project;
    lib_db_file = new QFile(project->getProjectPath() + "libraries.json");

    if(lib_db_file->exists()) {
        lib_db_file->open( QFile::ReadOnly );
        Log::write(QtInfoMsg, QObject::tr("LibraryManager"), QObject::tr("Reading Library Manager database.")); // TODO: move to internal debug/log system
        try {
            lib_db = json::parse( QString(lib_db_file->readAll()).toStdU32String() );
        } catch (const json::parse_error& e) {
            // project->throwMsgPopup(MSG_ERROR, QObject::tr("Libraries database parse error"), QObject::tr("Could not parse the library manager database. See logs for more information."));
            Popup::popup(QtWarningMsg, QObject::tr("LibraryManager"), QObject::tr("Libraries database parse error"), QObject::tr("Could not parse the library manager database. See logs for more information."));
            Log::write(QtWarningMsg, QObject::tr("LibraryManager"), QObject::tr("Libraries database parse error"), e.what()); // TODO: move to internal debug/log system
        }
    } else {
        lib_db_file->open( QFile::ReadWrite );
        Log::write(QtInfoMsg, QObject::tr("LibraryManager"), QObject::tr("Library Manager database doesn't exist. Creating it.")); // TODO: move to internal debug/log system
        lib_db_file->write("[]");
        lib_db = json::parse( "[]" );
    }

    lib_db_file->close();

    QDir tmp_dir(project->getProjectPath() + "libraries");
    if(!tmp_dir.exists()) {
        tmp_dir.mkdir(project->getProjectPath() + "libraries");
    }

    // TODO: Check libraries sanity (if lib in database has its folder)
}

LibManagerError LibraryManager::createLib(const QString &lib_name) {
    if(lib_name.isEmpty()) {
        return LM_BAD_NAME;
    }

    // TODO: check if lib doesnt already exist

    QDir lib_dir;
    // TODO: check if folder already exists
    lib_dir.mkdir(project->getProjectPath() + "libraries/" + lib_name);

    json tmp_json_array = {
        {"lib_name", lib_name.toStdString()},
        {"parent", ""},
        {"path", QString(project->getProjectPath() + "libraries/" + lib_name).toStdString()}
    };

    lib_db.push_back(tmp_json_array);
    lib_db_file->open(QIODeviceBase::WriteOnly | QIODevice::Text);
    QTextStream db_file_stream(lib_db_file);
    db_file_stream << QString::fromStdString(lib_db.dump(4));
    lib_db_file->close();

    return LM_SUCESS;
}

LibManagerError LibraryManager::createCell(const QString &lib_parent, const QString &cell_name) {
    if(lib_parent.isEmpty()) {
        return LM_BAD_NAME;
    }

    if(cell_name.isEmpty()) {
        return LM_BAD_NAME;
    }

    // TODO: check if cell/folder doesnt already exist
    QDir cell_dir;
    cell_dir.mkdir(project->getProjectPath() + "libraries/" + lib_parent + "/" + cell_name);

    return LM_SUCESS;
}
LibManagerError LibraryManager::createCellview(const QString &lib_parent, const QString &cell_parent,
                                               const QString &celview_type, const QString &cellview_name)
{
    if(lib_parent.isEmpty() || cell_parent.isEmpty() || celview_type.isEmpty() || cellview_name.isEmpty()) {
        return LM_BAD_NAME;
    }

    if(!LibraryManager::getCellviewTypes().contains(celview_type)) {
        return LM_BAD_CV_TYPE;
    }

    // TODO: check if cell/folder doesnt already exist

    QFile cellview_file(project->getProjectPath() + "libraries/" + lib_parent + "/" + cell_parent + "/" +
                       cellview_name + "." + cellviewTypesExt[celview_type]);
    cellview_file.open( QFile::ReadWrite );
    // TODO: create in a smarter way ? like with a funciton depending a type
    json tmp_json_array = {
        {"cellview_name", cellview_name.toStdString()},
        {"cellview_type", celview_type.toStdString()}
    };
    QTextStream tmp_fs(&cellview_file);
    tmp_fs << QString::fromStdString( tmp_json_array.dump(4) );
    cellview_file.close();

    return LM_SUCESS;
}


// GETTERS / SETTERS
json* LibraryManager::getDb(void) {
    return &(this->lib_db);
}
QStringList LibraryManager::getCellsFromLib(const QString lib_name) {
    QDir lib_dir;
    lib_dir.setPath(project->getProjectPath() + "libraries/" + lib_name);
    return lib_dir.entryList(QDir::Dirs | QDir::NoDot | QDir::NoDotDot);
}
int8_t LibraryManager::getCellviewsNameTypeFromCell(const QString lib_name, const QString cell_name, map<QString, QString> *cv_info) {
    QDir cell_dir;
    cell_dir.setPath(project->getProjectPath() + "libraries/" + lib_name + "/" + cell_name);

    //TODO: maybe here a json in the cell, listing the cellviews, would be cleaner. With a list of
    //      the cellnames, would avoid to strip the filenames here
    QStringList files = cell_dir.entryList(QDir::Files | QDir::NoDot | QDir::NoDotDot);

    cv_info->empty();

    for(auto it = files.begin(); it != files.end(); ++it) {
        //TODO: open file
        //  - check filename == filename in JSON
        //  - get type, add both in Qstring[2] list
        QFile cv_file(cell_dir.absoluteFilePath(*it));
        json file_json;

        //TODO: maybe split into smaller functions ? mostly for the checks

        if(cv_file.exists()) {
            cv_file.open( QFile::ReadOnly );

            try {
                file_json = json::parse( QString(cv_file.readAll()).toStdU32String() );
            } catch (const json::parse_error& e) {
                // TODO: write proper error log
                Log::write(QtWarningMsg, QObject::tr("LibraryManager"), QObject::tr("Error ..."));
                return 2;
            }

            if(!file_json.contains("cellview_name")) {
                // TODO: write proper error log
                Log::write(QtWarningMsg, QObject::tr("LibraryManager"), QObject::tr("Error ..."));
                return 3;
            }
            if(QString::fromStdString(file_json["cellview_name"]) != it->split(".")[0]) { // compare file name and name in json
                // TODO: write proper error log
                Log::write(QtWarningMsg, QObject::tr("LibraryManager"), QObject::tr("Error ..."));
                return 4;
            }
            if(!file_json.contains("cellview_type")) {
                // TODO: write proper error log
                Log::write(QtWarningMsg, QObject::tr("LibraryManager"), QObject::tr("Error ..."));
                return 5;
            }

            cv_info->insert({QString::fromStdString(file_json["cellview_name"]), QString::fromStdString(file_json["cellview_type"])});
        } else {
            // TODO: write proper error log
            Log::write(QtInfoMsg, QObject::tr("LibraryManager"), QObject::tr("Error ..."));
            return 1;
        }

        cv_file.close();
    }

    return 0;
}

map<QString, QString> LibraryManager::cellviewTypesExt = {{"schematic", "sch"}, {"layout", "lay"}, {"symbol", "sym"}};
QStringList LibraryManager::getCellviewTypes(void) {
    QStringList cellviewTypes;
    for(const auto& [type, ext] : cellviewTypesExt) {
        cellviewTypes.append(type);
    }
    return cellviewTypes;
}


LibManagerError LibraryManager::renameLib(const QString &original_name, const QString &new_name) {
    return LibManagerError::LM_SUCESS;
}
