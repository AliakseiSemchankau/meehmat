#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <string> 

using namespace std;

int main (){
  	std::string hostname;
  	std::cin >> hostname; // mech.math.msu.su
  	std::string address;
  	std::cin >> address; // http://mech.math.msu.su/~vvb/1course/index.html

    int s0 = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    struct hostent *host;
    host = gethostbyname(hostname.c_str());
    printf("host: %s", host->h_addr);
    sockaddr_in SockAddr;
    SockAddr.sin_port=htons(80);
    SockAddr.sin_family=AF_INET;
    SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

    std::cout << "Connecting...\n";
    if(connect(s0,(sockaddr*)(&SockAddr),sizeof(SockAddr)) != 0){
        cout << "Could not connect";
        system("pause");
        return 1;
    }
    std::cout << "Connected.\n";
    std::string request = "GET " + address + " HTTP/1.1\r\nHost: " + hostname + "\r\n\r\n";
    std::cout << "request = " << request << "\n";
    // send(s0,"GET http://mech.math.msu.su/~vvb/1course/index.html HTTP/1.1\r\nHost: mech.math.msu.su\r\n\r\n", strlen("GET http://mech.math.msu.su/~vvb/1course/index.html HTTP/1.1\r\nHost: mech.math.msu.su\r\n\r\n"),0);
    send(s0, request.c_str(), strlen(request.c_str()),0);
    
    char buffer[1];
    int nDataLength;

    FILE * fp;

    fp = fopen ("page.html", "w+");

    bool flag = false;

    while ((nDataLength = recv(s0,buffer,1,0)) > 0){        
        printf("%s", buffer);
        fprintf(fp, "%s", buffer);
    }

    int res = close(s0);
    if (res < 0) {
        perror("Cannot close"); exit(1);
    }

    fclose(fp);

    return 0;
}
