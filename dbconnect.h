#ifndef DBCONNECT_H
#define DBCONNECT_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

/*
    This file defines a helper function to open a connection to an
    in-memory SQLITE database and to create a test table.

    If you want to use another database, simply modify the code
    below. All the examples in this directory use this function to
    connect to a database.
*/
//! [0]
static bool createDBConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("note.db");
    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection.\n"
                     "This example needs SQLite support. Please read "
                     "the Qt SQL driver documentation for information how "
                     "to build it.\n\n"
                     "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    QSqlQuery query;
    return query.exec("CREATE TABLE IF NOT EXISTS tbl_note (id INTEGER PRIMARY KEY AUTOINCREMENT,\
                      title VARCHAR(20), describe VARCHAR(200),content TEXT, tags VARCHAR(100),\
                      updatetime TIMESTAMP DEFAULT (datetime(CURRENT_TIMESTAMP,'localtime')), \
                      createtime TIMESTAMP DEFAULT (datetime(CURRENT_TIMESTAMP,'localtime')));\
                      ");
}
//! [0]


#endif // DBCONNECT_H
