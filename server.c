#include <stdlib.h>
#include <stdint.h>

#include <netinet/in.h>
#include <stdio.h>

#include <errno.h>
#include <sys/socket.h>

#include "commons.h"

int init_unicast_socket(int bind_port) {
    // tworze socket
    int sockfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    
    // struktura definiujaca adres do bindowania
    struct sockaddr_in bind_addr;
    bind_addr.sin_family=AF_INET;
    bind_addr.sin_addr.s_addr=INADDR_ANY;
    bind_addr.sin_port=htons(bind_port);
    
    // bindowanie
    if(bind(sockfd,(struct sockaddr*)&bind_addr,sizeof(struct sockaddr_in))) {
        perror("bind(unicast)");
        exit(errno);
    }
    
    return sockfd;
}

int init_broadcast_socket(int bind_port) {
    // tworze socket
    int sockfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    
    // struktura definiujaca adres do bindowania
    struct sockaddr_in bind_addr;
    bind_addr.sin_family=AF_INET;
    bind_addr.sin_addr.s_addr=INADDR_ANY;
    bind_addr.sin_port=htons(bind_port);
    
    // bindowanie
    if(bind(sockfd,(struct sockaddr*)&bind_addr,sizeof(struct sockaddr_in))) {
        perror("bind(broadcast)");
        exit(errno);
    }
    
    return sockfd;
}

int init_multicast_socket(int bind_port) {
    // tworze socket
    int sockfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    
    // struktura definiująca grupę multikastową
    struct sockaddr_in mcast_addr;
    
    
    // struktura definiujaca adres do bindowania
    struct sockaddr_in bind_addr;
    bind_addr.sin_family=AF_INET;
    bind_addr.sin_addr.s_addr=INADDR_ANY;
    bind_addr.sin_port=htons(bind_port);
    
    // bindowanie
    if(bind(sockfd,(struct sockaddr*)&bind_addr,sizeof(struct sockaddr_in))) {
        perror("bind(multicast)");
        exit(errno);
    }
    
    return sockfd;
}

int main(void) {
    // tworze sockety
    int ucast_sockfd=init_unicast_socket(UNICAST_PORT);
    int bcast_sockfd=init_broadcast_socket(BROADCAST_PORT);
    int mcast_sockfd=init_multicast_socket(MULTICAST_PORT);
    
    // print
    fprintf(stdout,"Unicast sockfd: %d\nBroadcast sockfd: %d\nMulticast sockfd: %d\n", 
            ucast_sockfd,bcast_sockfd,mcast_sockfd);
    
    // dodaje deskryptory do fd_set
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(ucast_sockfd,&read_fds);
    FD_SET(bcast_sockfd,&read_fds);
    FD_SET(mcast_sockfd,&read_fds);
    
    // wyliczam maksymalna wartosc deskryptora do funkcji select()
    int maxfd=MAX(MAX(ucast_sockfd,bcast_sockfd),mcast_sockfd);
    
    // select()
    while(select(maxfd+1, &read_fds, NULL, NULL, NULL)) {
        if(FD_ISSET(ucast_sockfd,&read_fds)) {
            fprintf(stdout,"Dane przyszły na UNICAST\n");
            
            
        } else if(FD_ISSET(bcast_sockfd,&read_fds)) {
            fprintf(stdout,"Dane przyszły na BROADCAST\n");
            
        } else if(FD_ISSET(mcast_sockfd,&read_fds)) {
            fprintf(stdout,"Dane przyszły na MULTICAST\n");
            
        } else {
            fprintf(stderr,"coś niespodziewanego się stało...\n");
        }
        
        // ponowne wypełnienie zbioru deskryptorów
        FD_ZERO(&read_fds);
        FD_SET(ucast_sockfd,&read_fds);
        FD_SET(bcast_sockfd,&read_fds);
        FD_SET(mcast_sockfd,&read_fds);
    }
    
    return EXIT_SUCCESS;
}