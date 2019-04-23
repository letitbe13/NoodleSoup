#ifndef TEST_SERVER_H
#define TEST_SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QSqlDatabase>
#include <QtNetwork>

#include <http_helper.h>
#include "tinydb.h"
#include <fstream>
#include <fstab.h>

using namespace std;

class test_server : public QObject
{
    Q_OBJECT
public:
    explicit test_server(ushort port = 22, QString db_name = "db_test", QString logfile = "test_log.txt",QObject *parent = 0);
    QTimer *timer;
    ~test_server();
signals:
    void ServerTerminated();
    void ServiceTerminated(QString);
    void NewLogMessage(QString);

public slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnected();
    void slotServerDisconnected();
    void slotLog(QString);
    void TimerSlot();


private:
    QTcpServer * TCPServer;
    QTcpSocket * TCPSocket;
    ofstream myfile;
    QByteArray slotData;
    bool timeout;
    QString log_file;

    int REST_PARSE(QByteArray Request, QByteArray &Response, QVariantMap &Map);
    bool BodyParse_to_Map(const QByteArray &Body, QVariantMap &Map);

    TinyDB * db;

};

#endif // TEST_SERVER_H


