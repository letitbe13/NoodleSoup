#ifndef TINYDB_H
#define TINYDB_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include "qvariant.h"

class TinyDB : public QObject
{
    Q_OBJECT
public:
    explicit TinyDB(QString ConnectionName, QString db_name, QObject *parent = 0);
    bool SQLiteReadOneRow (QString table_name, uint id, QVariantList &FieldsList, QString &errString);
    int  SQLiteWriteOneRow(QString table_name, const QVariantList &FieldsList, QString &errString);
    bool isValid(){return Valid;}
    ~TinyDB();

signals:

public slots:

private:
    QSqlDatabase db;
    bool Valid;
};

#endif // TINYDB_H
