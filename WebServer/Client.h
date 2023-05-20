#pragma once
#include <string>

class Client
{
public:

    enum eReqMethod { GET = 0, HEAD, OPTIONS, POST, PUT, _DELETE, TRACE, UNSUPPORTED, NONE };
    enum eReqError { UnsupportedHeader, UnsupportedMethod, UnsupportedFormat, NotFound, MissingData, NOERR };

    std::string HEADER_HOST, HEADER_CONNECTION, MESSAGE, HEADER_ACCEPT, HEADER_ACCEPT_LANGUAGE, HEADER_ACCEPT_SET;
    std::string HEADER_CONTENT_TYPE, HOST, HTTP_QUERY, HTTP_BODY, PATH, DATE, RESPONSE_MESSAGE, BODY_HANDLER;
    eReqError HTTP_ERROR;
    eReqMethod HTTP_METHOD;
    size_t HEADER_CONTENT_LENGTH;
    int readCursor;

    void resetRequest() {
        HEADER_HOST.clear();
        HEADER_ACCEPT.clear();
        HEADER_ACCEPT_LANGUAGE.clear();
        PATH.clear();
        HEADER_CONNECTION.clear();
        HEADER_CONTENT_TYPE.clear();
        HEADER_CONTENT_LENGTH = 0;
        HTTP_METHOD = NONE;
        HTTP_ERROR = NOERR;
        MESSAGE.clear();
        HTTP_QUERY.clear();
        readCursor = 0;
    }
};

