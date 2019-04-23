#include "test_server.h"

using namespace std;

test_server::test_server(ushort port, QString db_name, QString logfile, QObject *parent) :
    QObject(parent)
{
    this->log_file = logfile;

    TCPServer = new QTcpServer(this);
    connect(TCPServer,  &QTcpServer::newConnection,     this, &test_server::slotNewConnection);
    connect(this,       &test_server::ServerTerminated, this, &test_server::slotServerDisconnected,Qt::QueuedConnection);
    connect(this,       &test_server::NewLogMessage,    this, &test_server::slotLog,Qt::QueuedConnection);

    timer = new QTimer(this);
    timer->setSingleShot(true);

    connect(timer, &QTimer::timeout,this, &test_server::TimerSlot);

    db = new TinyDB("MY", db_name, this);

    if(db->isValid()){
        emit(NewLogMessage("DB opened"));
    }else{
        emit(NewLogMessage("Can't open DB."));
        emit(ServerTerminated());
        return;
    }

    if(!TCPServer->listen(QHostAddress::Any, port)){
           emit(NewLogMessage(TCPServer->errorString()));
           emit(ServerTerminated());
           return;
    } else {
            emit(NewLogMessage("server start"));
    }
    slotData.clear();
}

void test_server::slotNewConnection()
{
    TCPSocket = TCPServer->nextPendingConnection();

    connect(TCPSocket, &QTcpSocket::readyRead, this, &test_server::slotServerRead);
    connect(TCPSocket, &QTcpSocket::disconnected, this, &test_server::slotClientDisconnected);
}

void test_server::slotServerRead()
{
    while(TCPSocket->bytesAvailable()>0 || timeout){

            slotData+=TCPSocket->readAll();

            if(!timer->isActive()){
                timer->start(2000);
            }

            QByteArray Response;
            QVariantMap Map;

            int mode = HTTP_Helper::REST_PARSE(slotData,Response,Map);

            if(mode == POST_REQUEST){
                QString test  = Map["text"].toString();

                QString errString;
                QVariantList FieldsList;
                FieldsList.append(Map["text"]);
                FieldsList.append(Map["value"]);

                int newID;
                if((newID = db->SQLiteWriteOneRow("testdata", FieldsList,errString))==-1){

                    QString postDataSize = QByteArray::number(errString.size());
                    Response = QString(Response).arg(postDataSize,errString).toUtf8();

                }else{
                    QString info = "ID = " + QVariant(newID).toString();
                    QString postDataSize = QByteArray::number(info.size());
                    Response = QString(Response).arg(postDataSize,info).toUtf8();
                }
                TCPSocket->write(Response);
                TCPSocket->disconnectFromHost();
                slotData.clear();
                timer->stop();

            }else if(mode == GET_REQUEST){
                if(Map.contains("id")){
                    QString errString = "";
                    QVariantList FieldsList;
                    if(db->SQLiteReadOneRow("testdata",Map["id"].toInt(), FieldsList,errString)){
                        QJsonObject json;

                        if(FieldsList.size()>=3){
                            json.insert("id",   FieldsList[0].toInt());
                            json.insert("text", FieldsList[1].toString());
                            json.insert("value", FieldsList[2].toString());
                        }

                        QByteArray jsonstring = QJsonDocument(json).toJson();
                        QString postDataSize = QByteArray::number(jsonstring.size());
                        Response = QString(Response).arg(postDataSize,QString(jsonstring)).toUtf8();
                    }else{
                        QString postDataSize = QByteArray::number(errString.size());
                        Response = QString(Response).arg(postDataSize,errString).toUtf8();
                    }

                }else if(Map.contains("stop")){
                    QString info = "Goodbye!";
                    QString postDataSize = QByteArray::number(info.size());
                    Response = QString(Response).arg(postDataSize,info).toUtf8();
                    emit(ServerTerminated());
                }
                TCPSocket->write(Response);
                TCPSocket->disconnectFromHost();
                slotData.clear();
                timer->stop();

            }else if((mode == WAITING && timeout) || mode == BAD_REQUEST){
                Response = QString(Response).toUtf8();
                TCPSocket->write(Response);
                TCPSocket->disconnectFromHost();
                slotData.clear();
                timer->stop();
                timeout = false;
            }
       }
}

void test_server::slotClientDisconnected()
{
    TCPSocket->close();
}

void test_server::slotServerDisconnected()
{
    emit(NewLogMessage("server stop\n\n"));
    TCPServer->close();
    myfile.close();
    emit(ServiceTerminated("Client termination request"));
}

void test_server::TimerSlot()
{
    timeout = true;
    slotServerRead();
    timer->stop();
    slotData.clear();
}

void test_server::slotLog(QString message)
{
    if(!myfile.is_open())
        myfile.open(log_file.toStdString(),ios_base::app);
    QDateTime dt = QDateTime::currentDateTime();
    myfile << dt.toString().toStdString()<<":   "<<message.toStdString()<<"\n";
    myfile.close();
}

test_server::~test_server()
{
    TCPServer->deleteLater();
    TCPSocket->deleteLater();
    timer->deleteLater();
}

