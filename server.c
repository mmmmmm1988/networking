#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/param.h>
#include <errno.h>

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
    if(!bind(sockfd,&bind_addr,sizeof(struct sockaddr_in))) {
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
    if(!bind(sockfd,&bind_addr,sizeof(struct sockaddr_in))) {
        perror("bind(broadcast)");
        exit(errno);
    }
    
    return sockfd;
}

int init_multicast_socket(int bind_port) {
    // tworze socket
    int sockfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    
    // struktura definiujaca adres do bindowania
    struct sockaddr_in bind_addr;
    bind_addr.sin_family=AF_INET;
    bind_addr.sin_addr.s_addr=INADDR_ANY;
    bind_addr.sin_port=htons(bind_port);
    
    // bindowanie
    if(!bind(sockfd,&bind_addr,sizeof(struct sockaddr_in))) {
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
        
    }
    
    return EXIT_SUCCESS;
}