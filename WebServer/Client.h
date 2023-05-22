#pragma once
#include <string>

class Client
{
public:

    enum eReqMethod { GET = 0, HEAD, OPTIONS, POST, PUT, H_DELETE, TRACE, UNSUPPORTED, NONE };
    enum eReqError { UnsupportedHeader, UnsupportedMethod, UnsupportedFormat, NotFound, MissingData, NOERR };

    // HEADERS
    std::string HEADER_HOST, HEADER_CONNECTION, HEADER_ACCEPT, HEADER_ACCEPT_LANGUAGE, HEADER_ACCEPT_SET, HEADER_CONTENT_TYPE;

    // NON-HEADERS
    std::string MESSAGE, HOST, HTTP_QUERY, HTTP_BODY, PATH, DATE, RESPONSE_MESSAGE, BODY_HANDLER;

    eReqError HTTP_ERROR;
    eReqMethod HTTP_METHOD;
    size_t HEADER_CONTENT_LENGTH;
    int readPointer;

    void resetRequest() {
        PATH.clear();
        HEADER_CONTENT_TYPE.clear();
        HEADER_HOST.clear();
        HEADER_ACCEPT.clear();
        HEADER_ACCEPT_LANGUAGE.clear();
        HEADER_CONNECTION.clear();
        HEADER_CONTENT_LENGTH = 0;
        HTTP_METHOD = NONE;
        HTTP_ERROR = NOERR;
        MESSAGE.clear();
        HTTP_QUERY.clear();
        readPointer = 0;
    }

    void getRequest();
    void getHeader();
    void cleanRequest();
    void cleanResponse();
};

