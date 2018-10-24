//
//  server.c
//  
//
//  Created by Jenny Zhu on 10/24/18.
//

/* A simple server in the internet domain using TCP
 The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[1024];
    char nbuffer[1024];
    int sum = 0;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc != 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                       (struct sockaddr *) &cli_addr,
                       &clilen);
    if (newsockfd < 0)
        error("ERROR on accept");
    while (1){
        bzero(buffer,1024);
        int buflen = strlen(buffer);
        n = read(newsockfd,buffer,1024);
        if (n < 0) error("ERROR reading from socket");
        
        //check if buffer only contains numbers
        for(int i = 0; i <buflen ; i++){
            if(isalpha(buffer[i])){
                //printf("From server: Sorry, cannot compute!\n");
               // char errormsg[] = "From server: Sorry, cannot compute!";
                write(newsockfd, "From server: Sorry, cannot compute!", 35);
                continue;
            }
        }
        //calculate
        
        if (buflen == 1){
            write(newsockfd, buffer, 1024);
            exit(0);
        }
        
        while(buflen > 1){
            for (int i = 0; i < buflen; i++)
            {
                sum += buffer[i] - '0';
            }
            int m = 0;
            while (sum)
            {
                nbuffer[m] = (sum % 10) + '0';
                sum /= 10;
                ++m;
            }
            bzero(buffer, 1024);
            for (int i = m - 1, j = 0; i >= 0; --i, ++j)
            {
                buffer[j] = nbuffer[i];
            }
            buflen = strlen(buffer);
            write(newsockfd, buffer, buflen);
            ///
        }
    /*printf("Here is the message: %s\n",buffer);
    n = write(newsockfd,"I got your message",18);
    if (n < 0) error("ERROR writing to socket");
    close(newsockfd);
    close(sockfd);*/
    }
    return 0;
}
