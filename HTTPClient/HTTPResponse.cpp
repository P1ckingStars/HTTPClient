//
//  HTTPResponse.cpp
//  HTTPClient
//
//  Created by 王子豪 on 8/27/20.
//  Copyright © 2020 王子豪. All rights reserved.
//

#include "HTTPResponse.hpp"
#include <unistd.h>

int indexOf(char c, char const *line) {
    size_t size = strlen(line);
    for (int index = 0; index < size; index++) {
        if (line[index] == c) {
            return index;
        }
    }
    return -1;
}

char *readLine(int fd, int maxLen) {
    char c;
    char *buff = (char *)malloc(maxLen);
    bzero(buff, maxLen);
    for (int index = 0; read(fd, &c, 1) == 1 && index < maxLen; index++) {
        if (c == '\r') {
            if (read(fd, &c, 1)) {
                if (c == '\n') {
                    break;
                } else {
                    buff[index++] = '\r';
                    buff[index] = c;
                }
            } else {
                buff[index] = '\r';
                break;
            }
        }
        buff[index] = c;
    }
    return buff;
}

char *readShortline(int fd) {
    return readLine(fd, 1024);
}

char *readLongline(int fd) {
    return readLine(fd, 8192);
}

void HTTPResponse::setResponseLine(char *line) {
    char *statusStr = (char *)malloc(4);
    bzero(statusStr, 4);
    memcpy(statusStr, line, 3);
    int status = atoi(statusStr);
    printf("status1: %d\n", status);
    this->responseLine.status = status;
}

ResponseLine::ResponseLine(char *version, int status, char *description) {
    this->version = version;
    this->status = status;
    this->description = description;
}

HTTPResponse::HTTPResponse(int sockfd) {
    char *statusLine = readShortline(sockfd);
    setResponseLine(statusLine);
    char *headerLines;
    map<char *, char *> header;
    while (strlen(headerLines = readShortline(sockfd)) > 0) {
        int index = indexOf(' ', headerLines);
        char *argn = (char *)malloc(index+1);
        bzero(argn, index+1);
        memcpy(argn, headerLines, index);
        size_t size = strlen(headerLines+index+1);
        char *argv = (char *)malloc(size+1);
        bzero(argv, size);
        memcpy(argv, headerLines+index+1, size);
        header[argn] = argv;
    }
    if (header["content-size"] == NULL) {
//        body = readLongline(sockfd);
        body = (char *)malloc(8192);
        bzero(body, 8192);
        read(sockfd, body, 8191);
    } else {
        int size = atoi(header["content-size"]);
        body = (char *)malloc(size+1);
        bzero(body, size+1);
        read(sockfd, body, size);
    }
}

char *HTTPResponse::getBody() {
    return body;
}
