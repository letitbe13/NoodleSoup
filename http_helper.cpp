#include "http_helper.h"

int HTTP_Helper::REST_PARSE(QByteArray Request, QByteArray &Response, QVariantMap &Map)
{
    QString RequestType = QString(Request).split("\n").at(0).split(" ").at(0);
    Response = "HTTP/1.1 %1 %2\r\n\Server: Test_Server\r\n\"Content-Length: %3\r\n\"Connection: close\r\n\r\n\%4";
    Map.clear();

    if(RequestType == "POST"){
        QByteArray Body;
        int mode;
        if((mode = GET_BODY(Request,Body))==SUCCES){
            POST_PARSE_ToMap(Body, Map);
            Response = QString(Response).arg("200","OK").toUtf8();
            return POST_REQUEST;
        }else if(mode == BAD_REQUEST){

            Response = QString(Response).arg("400","Request is corrupted","0","Request is corrupted").toUtf8();
            return BAD_REQUEST;
        }else if(mode == WAITING){
            Response = QString(Response).arg("400","Request is corrupted","0","Request is corrupted").toUtf8();
            return WAITING;
        }
    }else if(RequestType == "GET"){

        QStringList FullPath = QString(Request).split("\n").at(0).split(" ").at(1).split("/");
        if(FullPath.count()==3 && FullPath[1]=="id"){
            bool ok;
            FullPath[2].toInt(&ok);
            if(ok){
                int id =  FullPath[2].toInt();
                Map["id"]=id;
                Response = QString(Response).arg("200","OK").toUtf8();
                return GET_REQUEST;
            }else{
                Response = QString(Response).arg("400","Incorrect ID format","0","Incorrect ID format").toUtf8();
                return BAD_REQUEST;
            }
        }else if(FullPath.count()==2 && FullPath[1]=="stop"){
            Map["stop"]=true;
            Response = QString(Response).arg("200","OK").toUtf8();
            return GET_REQUEST;
        }
        else{
            Response = QString(Response).arg("400","Incorrect Path","0","Incorrect Path").toUtf8();
            return BAD_REQUEST;
        }
    }else{
        Response = QString(Response).arg("400","Incorrect request type","0","Incorrect request type").toUtf8();
        return BAD_REQUEST;
    }
    return BAD_REQUEST;
}


void HTTP_Helper::POST_PARSE_ToMap(const QByteArray &Body, QVariantMap &Map)
{
    QByteArray body = Body;
    body.replace("+"," ");
    QStringList pairs = QString(body).split("&");
    while(!pairs.isEmpty()){
        QStringList pair = pairs.takeFirst().split("=");
        if(pair.count()==2){
                Map[QString(pair[0])]=QString(pair[1]);
        }
    }
}

int HTTP_Helper::GET_BODY(const QByteArray &Request, QByteArray &Body)
{

    QRegExp rx("Content-Length: (\\d+)\\r");
    int pos = rx.indexIn(QString(Request));
    if(pos>=0){
        int BodyLength = rx.cap(1).toInt();
        int HeadersLength = Request.indexOf("\r\n\r\n")+4;
        if(Request.size() - HeadersLength == BodyLength){

            Body = Request;
            Body.remove(0,HeadersLength);
            return SUCCES;

        }else if(Request.size() - HeadersLength < BodyLength){
            return WAITING;
        }
    }
    return BAD_REQUEST;
}
