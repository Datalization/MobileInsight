#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <poll.h>
#include <fcntl.h>

int sockfd;
int buffer_size = 2048;
int newsockfd;
unsigned int clilen;
struct pollfd pollfds[1];

int main()
{
    int port_number= 20;
    struct sockaddr_in serv_addr,cli_addr;
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        fprintf(stderr,"error when accept:%s\n",strerror(errno));
        exit(1);
    }  
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_number);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr,"error when binding\n");
        close(sockfd);
        exit(1);
    }
    listen(sockfd,5);    
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);   
    if(newsockfd<0)
    {
        fprintf(stderr,"error when accept:%s\n",strerror(errno));
        close(sockfd);
        exit(1);
    }
    pollfds[0].fd = newsockfd;
    pollfds[0].events = POLLIN;
    while (1)
    {
        int ret = poll(pollfds,1,0);
        if (ret< 0)
        {
            char* error  = strerror(errno);
            fprintf(stderr,"error when poll: %s \n",error);
            exit(1);
        }
        if(ret==0)
            continue;    
        if(pollfds[0].revents & POLLIN)
        {
            int tempread= 0 ;
            char buf[buffer_size];
            tempread=read(newsockfd,buf,buffer_size);
            if(tempread<0)
            {
                fprintf(stderr,"error when read:%s \n",strerror(errno));
                exit(1);
            }
            write(STDOUT_FILENO,buf,tempread);
        }
        if(pollfds[0].revents & POLLERR)
        {
            fprintf(stderr,"error occurred in stdin \n");
            break;
        }
        if(pollfds[0].revents & POLLHUP)
        {
            fprintf(stderr,"error occurred in stdin \n");
            break;
        }
    }
    exit(0);
}