#include "tinydb.h"


TinyDB::TinyDB(QString ConnectionName, QString db_name,QObject *parent) :
    QObject(parent)
{
    QString d = db_name;
    this->ConnectionName = ConnectionName;
    db = QSqlDatabase::addDatabase("QSQLITE",ConnectionName);
    db.setDatabaseName(db_name);
    Valid = db.open();
    db.close();
}

bool TinyDB::SQLiteReadOneRow(QString table_name, uint id, QVariantList &FieldsList, QString &errString)
{
    errString.clear();
    db.database("MY");
    if(db.isValid()){
        QString s = db.databaseName();
        db.setDatabaseName(s);
        if(db.open()){

            QSqlQuery query(db);
            QString sql = "SELECT * FROM %1 where id = %2";
            sql = sql.arg(table_name,QVariant(id).toString());

            if(!query.exec(sql)){
                errString = query.lastError().text();
            }else{
                 FieldsList.clear();
                if(query.next()){
                        FieldsList.insert(0,query.value(0));
                        FieldsList.insert(1,query.value(1));
                        FieldsList.insert(2,query.value(2));
                }else{
                    errString = "ID wasn't found";
                }
            }
            db.close();
        }else{errString = db.lastError().text();}
    }else{errString = db.lastError().text();}

    return errString.isEmpty();

}

int  TinyDB::SQLiteWriteOneRow(QString table_name, const QVariantList &FieldsList, QString &errString)
{
    errString.clear();
    int newID = -1;
    db.database(ConnectionName);
    if(db.isValid() && FieldsList.count() >= 2){

        db.setDatabaseName(db.databaseName());
        if(db.open()){

            QSqlQuery query(db);
            QString sql = "INSERT INTO %1 (text,value) VALUES(\"%2\",%3)";
            sql = sql.arg(table_name,FieldsList[0].toString(),FieldsList[1].toString());

            if(!query.exec(sql)){
                errString = query.lastError().text();
            }
            newID = query.lastInsertId().toInt();
            db.close();
        }else{errString = db.lastError().text();}
    }else{errString = db.lastError().text();}


    return errString.isEmpty()? newID : -1;

}

TinyDB::~TinyDB()
{
    db.removeDatabase(ConnectionName);
}
