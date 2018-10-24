//
//  server.c
//  
//
//  Created by Jenny Zhu on 10/24/18.
//

/* Creates a datagram server.  The port
 number is passed as an argument.  This
 server runs forever */

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <ctype.h>
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sock, length, n;
    socklen_t fromlen;
    struct sockaddr_in server;
    struct sockaddr_in from;
    char buffer[1024];
    int sum = 0;
    char nbuffer[1024];
    if (argc != 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(0);
    }
    
    sock=socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) error("Opening socket");
    length = sizeof(server);
    bzero(&server,length);
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(atoi(argv[1]));
    if (bind(sock,(struct sockaddr *)&server,length)<0)
        error("binding");
    fromlen = sizeof(struct sockaddr_in);
    while (1) {
        bzero(buffer, 1024);
        int buflen = strlen(buffer);
        n = recvfrom(sock,buffer,1024,0,(struct sockaddr *)&from,&fromlen);
        if (n < 0) error("recvfrom");
        //check if buffer only contains numbers
        for(int i = 0; i < buflen; i++){
            if(isalpha(buffer[i])){
                //printf("From server: Sorry, cannot compute!\n");
                char errormsg[] = "From server: Sorry, cannot compute!";
                sendto(sock, errormsg, 35, 0, (struct sockaddr *)&from,fromlen);
                continue;
            }
        }
        
        //calculate
        
        if (buflen == 1){
            sendto(sock, buffer, 1024, 0, (struct sockaddr *)&from, fromlen);
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
            sendto(sock, buffer, buflen, 0, (struct sockaddr *)&from, fromlen);
            ///
        }
        /*write(1,"Enter string: ",14);
        write(1,buffer,n);
        n = sendto(sock,"Got your message\n",17,
                   0,(struct sockaddr *)&from,fromlen);
        if (n  < 0) error("sendto");*/
    }
    return 0;
}
