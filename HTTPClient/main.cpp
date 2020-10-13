//
//  main.cpp
//  HTTPClient
//
//  Created by 王子豪 on 8/27/20.
//  Copyright © 2020 王子豪. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"

int main(int argc, const char * argv[]) {
    HTTPRequest *request = new HTTPRequest("www.google.com", RequestLine(GET, "/", "1.1"), map<char *, char *>(), NULL);
    HTTPResponse *response = request->sendRequest();
    
    printf("status: %d\n", response->responseLine.status);
    printf(response->getBody());
    return 0;
}
