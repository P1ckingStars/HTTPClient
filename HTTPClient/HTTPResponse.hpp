//
//  HTTPResponse.hpp
//  HTTPClient
//
//  Created by 王子豪 on 8/27/20.
//  Copyright © 2020 王子豪. All rights reserved.
//

#ifndef HTTPResponse_hpp
#define HTTPResponse_hpp
#include <map>
#include <stdio.h>
using namespace std;

struct ResponseLine {
    ResponseLine(char *version, int status, char *description);
    ResponseLine() {    }
    char *version = NULL;
    int status = 0;
    char *description = NULL;
};

class HTTPResponse {
public:
    HTTPResponse(int sockfd);
    char *getBody();
//private:
    void setResponseLine(char *line);
    ResponseLine responseLine;
    map<char *, char *> header;
    char *body;
};
#endif /* HTTPResponse_hpp */
