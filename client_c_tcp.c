//
//  client.c
//  
//
//  Created by Jenny Zhu on 10/24/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    char buffer[1024];
    if (argc != 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    //memset(serv_addr.sin_zero, '\0', sizeof(serv_addr.sin_zero));
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    bzero(buffer,1024);
    printf("Enter string: %s", buffer);
    //bzero(buffer,1024);
    fgets(buffer,1024,stdin);
    n = write(sockfd,buffer,strlen(buffer)-1);
    if (n < 0)
        error("ERROR writing to socket");
    
    //check buffer
    for(int i = 0; i < n; i++){
        if(isalpha(buffer[i])){
            printf("From server: Sorry, cannot compute!\n");
            //char errormsg[] = "From server: Sorry, cannot compute!";
            //sendto(sock, errormsg, 35, 0, (struct sockaddr *)&server,length);
            exit(1);
        }
    }
    
    //calculate
    n = 1024;
    while(n!=1){
        bzero(buffer, 1024);
        n =read(sockfd,buffer,1024);
        printf("From server: %s\n", buffer);
    }

    /*bzero(buffer,1024);
    n = read(sockfd,buffer,1023);
    if (n < 0)
        error("ERROR reading from socket");
    printf("%s\n",buffer);*/
    //close(sockfd);
    return 0;
}
