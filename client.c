#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <memory.h>
#include <arpa/inet.h>

#include "commons.h"

int init_socket(int bind_port) {
    int sockfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    
    if(bind_port) { // czy bindujemy?
        struct sockaddr_in bind_addr;
        bind_addr.sin_family=AF_INET;
        bind_addr.sin_addr.s_addr=INADDR_ANY;
        bind_addr.sin_port=htons(bind_port);

        if(bind(sockfd,(struct sockaddr*)&bind_addr,sizeof(struct sockaddr_in))) { // czy zbindowano?
            perror("bind(broadcast)");
            exit(errno);
        }
    }
            
    return sockfd;
}

int main(void) {
    
    int sockfd=init_socket(0);
    
    struct sockaddr_in target_addr;
    target_addr.sin_family=AF_INET;
    target_addr.sin_port=htons(31337);  // port docelowy
    target_addr.sin_addr.s_addr=inet_addr("127.0.0.1"); // docelowy host
    
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO,&read_fds);
    
    uint8_t* buffer=(uint8_t*)malloc(MAX_BUFF_SIZE);
    int maxfd=MAX(STDIN_FILENO,0);
    
    while(select(maxfd+1,&read_fds,NULL,NULL,NULL)) {
        memset(buffer,0,MAX_BUFF_SIZE);
        
        if(FD_ISSET(STDIN_FILENO,&read_fds)) {
            int bytes_read=read(STDIN_FILENO,buffer,MAX_BUFF_SIZE); // czytam
            int bytes_sent=sendto(sockfd,buffer,bytes_read,0,
                    (struct sockaddr*)&target_addr,sizeof(struct sockaddr)); // wysylam
            
            if(bytes_read==bytes_sent) {    // sprawdzam
                fprintf(stdout,"> %s", buffer);
            } else {    // błąd
                fprintf(stderr,"Coś poszło nie tak read=%d, sent=%d\n",bytes_read,bytes_sent);
            }
            
        } else {
            fprintf(stderr,"Coś nieprzewidzianego sie dzieje\n");
        }
        
        // ustawiam fdset
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO,&read_fds);
    }
    
    free(buffer);
    
    return EXIT_SUCCESS;
}