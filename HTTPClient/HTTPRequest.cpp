//
//  HTTPRequest.cpp
//  HTTPClient
//
//  Created by 王子豪 on 8/27/20.
//  Copyright © 2020 王子豪. All rights reserved.
//

#include "HTTPRequest.hpp"
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

char const *HTTPRequest::getRequestLine() {
    char *buff = (char *)malloc(1024);
    bzero(buff, 1024);
    char *slice;
    switch (requestLine.method) {
        case GET:
            memcpy(buff, "GET ", 4);
            slice = buff+4;
            break;
        case POST:
            memcpy(buff, "POST ", 5);
            slice = buff+5;
            break;
        case HEAD:
            memcpy(buff, "HEAD ", 5);
            slice = buff+5;
            break;
        default:
            break;
    }
    memcpy(slice, requestLine.url, strlen(requestLine.url));
    memcpy(slice+strlen(requestLine.url), " HTTP/", 6);
    memcpy(slice+6+strlen(requestLine.url), requestLine.version, 3);
    return buff;
}

char const *HTTPRequest::getHeader() {
    char *buff = (char *)malloc(MAX_HEADER_LEN+1);
    bzero(buff, MAX_HEADER_LEN+1);
    map<char *, char *>::iterator iter;
    int index = 0;
    for (iter = header.begin();
         iter != header.end() && index < MAX_HEADER_LEN;
         iter++) {
        memcpy(buff+index, iter->first, strlen(iter->first));
        index += strlen(iter->first);
        buff[index++] = ' ';
        memcpy(buff+index, iter->second, strlen(iter->second));
        index += strlen(iter->second);
        buff[index++] = '\r';
        buff[index++] = '\n';
    }
    return buff;
}

char const *HTTPRequest::getBody() {
    return body;
}

int HTTPRequest::setRequestLine(RequestLine request) {
    this->requestLine = request;
    return 0;
}

int HTTPRequest::setHeader(map<char *, char*> header) {
    this->header = header;
    return 0;
}

int HTTPRequest::setBody(char *body) {
    if (requestLine.method == POST) {
        char *buff = (char *)malloc(strlen(body)+1);
        bzero(buff, strlen(body)+1);
        memcpy(buff, body, strlen(body));
        this->body = buff;
        return 0;
    }
    return -1;
}

HTTPResponse *HTTPRequest::sendRequest() {
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return NULL;
    }
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    struct hostent *host = gethostbyname(requestLine.url);
    if((host = gethostbyname(domain)) == NULL) {
        fprintf(stderr,"不能得到IP/n");
        return NULL;
    }
    char *ip = inet_ntoa(*(struct in_addr*)host->h_addr_list[0]);
    sockaddr.sin_addr.s_addr = inet_addr(ip);
    sockaddr.sin_port = htons(80);
    printf("%s\n", ip);
    int err = connect(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    if (err < 0) {
        perror("connect");
        return NULL;
    }
    const char *request = this->getRequestLine();
    const char *header = this->getHeader();
    const char *body = getBody();
    printf("Length: %d\r\n %s\n", strlen(request), request);
    if (write(sockfd, request, strlen(request)) != strlen(request)) {
        printf("something wrong");
    }
    if (strlen(header) != 0) {
        write(sockfd, "\r\n", 2);
        write(sockfd, header, sizeof(header));
    }
    write(sockfd, "\r\n\r\n", 4);
    if (body != NULL) {
        write(sockfd, body, strlen(body));
        write(sockfd, "\r\n\r\n", 4);
    }
    return new HTTPResponse(sockfd);
}

RequestLine::RequestLine(Method method, char *url, char *version) {
       this->method = method;
       this->url = url;
       this->version = version;
   }

HTTPRequest::HTTPRequest(char *domain, RequestLine requestLine, map<char *, char *> header, char *body) {
    this->domain = domain;
    this->requestLine = requestLine;
    this->header = header;
    this->body = NULL;
    this->setBody(body);
}
