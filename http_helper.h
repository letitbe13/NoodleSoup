#ifndef HTTP_HELPER_H
#define HTTP_HELPER_H

#include <qbytearray.h>
#include <qvariant.h>

#define BAD_REQUEST     0
#define POST_REQUEST    1
#define GET_REQUEST     2
#define WAITING         3
#define SUCCES          4

class HTTP_Helper
{
public:
    HTTP_Helper(){};
    static int REST_PARSE(const QByteArray Request, QByteArray &Response, QVariantMap &Map);
    static void POST_PARSE_ToMap(const QByteArray &Body, QVariantMap &Map);
    static int GET_BODY(const QByteArray &Request, QByteArray &Body);
};

#endif // HTTP_HELPER_H
