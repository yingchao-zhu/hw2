//
//  client.c
//  
//
//  Created by Jenny Zhu on 10/24/18.
//

/* UDP client in the internet domain */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
void error(const char *);
int main(int argc, char *argv[])
{
    int sock, n;
    unsigned int length;
    struct sockaddr_in server;
    struct hostent *hp;
    char buffer[1024];
    
    if (argc != 3) { printf("Usage: server port\n");
        exit(1);
    }
    sock= socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) error("socket");
    
    server.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if (hp==0) error("Unknown host");
    
    bcopy((char *)hp->h_addr,
          (char *)&server.sin_addr,
          hp->h_length);
    server.sin_port = htons(atoi(argv[2]));
    memset(server.sin_zero, '\0', sizeof(server.sin_zero));
    length=sizeof(struct sockaddr_in);
    
    bzero(buffer, 1024);
    
    printf("Enter string: %s", buffer);
    //bzero(buffer,256);
    fgets(buffer,1024,stdin);
    n=sendto(sock,buffer, strlen(buffer)-1,0,(const struct sockaddr *)&server,length);
    if (n < 0) error("Sendto");
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
    while(n>1){
        bzero(buffer, 1024);
        n =recvfrom(sock,buffer,1024,0,NULL, NULL);
        printf("From server: %s\n", buffer);
    }
    //close(sock);
    return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
