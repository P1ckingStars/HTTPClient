//
//  HTTPRequest.hpp
//  HTTPClient
//
//  Created by 王子豪 on 8/27/20.
//  Copyright © 2020 王子豪. All rights reserved.
//

#ifndef HTTPRequest_hpp
#define HTTPRequest_hpp
#include "HTTPResponse.hpp"
#include <stdio.h>
#include <map>
#define MAX_HEADER_LEN 8192
using namespace std;

enum Method {
    GET, POST, HEAD
};

struct RequestLine {
public:
    RequestLine(Method method, char *url, char *version);
    Method method;
    char *url;
    char *version;
};

class HTTPRequest {
private:
    char *domain;
    RequestLine requestLine = RequestLine(GET, NULL, NULL);
    map<char *, char *> header;
    char *body;
public:
    HTTPRequest(char *domain, RequestLine requestLine, map<char *, char *> header, char *body);
    char const *getRequestLine();
    int setRequestLine(RequestLine request);
    char const *getHeader();
    int setHeader(map<char *, char *> header);
    char const *getBody();
    int setBody(char *body);
    HTTPResponse *sendRequest();
};
#endif /* HTTPRequest_hpp */
