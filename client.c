#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <memory.h>

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
    
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO,&read_fds);
    
    uint8_t* buffer=(uint8_t*)malloc(MAX_BUFF_SIZE);
    int maxfd=MAX(STDIN_FILENO,0);
    
    while(select(maxfd+1,&read_fds,NULL,NULL,NULL)) {
        memset(buffer,0,MAX_BUFF_SIZE);
        
        if(FD_ISSET(STDIN_FILENO,&read_fds)) {
            read(STDIN_FILENO,buffer,MAX_BUFF_SIZE);
            
            
            fprintf(stdout,"> %s", buffer);
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