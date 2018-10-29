#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <poll.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "zlib.h"

struct pollfd pollfds[2];
int buffer_size = 2048;
int sockfd;

int main()
{
    int port_number = 20;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server= gethostbyname("localhost"); 
    memset((char *) &serv_addr, 0,sizeof(serv_addr));
    serv_addr.sin_port = htons(port_number);
    memcpy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);    
    serv_addr.sin_family = AF_INET;
    if (connect(sockfd , (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr,"error:connect failed");
        exit(1);
    }

    pollfds[0].fd=STDIN_FILENO;
    pollfds[0].events = POLLIN;

    while(1)
    {
        int ret;
        ret = poll(pollfds,2,0);
        if(ret==0)
            continue;
        if (pollfds[0].revents&POLLIN)
        {
            int tempread= 0 ;
            char buf[buffer_size];
            tempread=read(STDIN_FILENO,buf,buffer_size);
            write(sockfd,buf,tempread);
        }            
        if(pollfds[0].revents&POLLERR)
        {
            fprintf(stderr,"error in stdin\n");
            break;
        }
        if(pollfds[0].revents&POLLHUP)
        {
            fprintf(stderr,"error in stdin\n");
            break;
        }
    exit(0);
    }
}