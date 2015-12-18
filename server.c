#include <stdlib.h>
#include <stdint.h>

#include <netinet/in.h>
#include <stdio.h>

#include <errno.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>

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
    int ucast_sockfd=init_unicast_socket(UNICAST_SERVER_PORT);
    int bcast_sockfd=init_broadcast_socket(BROADCAST_SERVER_PORT);
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
    
    // tworze bufor na dane
    uint8_t* buffer=calloc(MAX_BUFF_SIZE,sizeof(uint8_t));
    
    // adres xrodlowy
    struct sockaddr_in src_addr;
    socklen_t sockaddr_len=sizeof(struct sockaddr_in);
//    memset(&src_addr,0,sizeof(struct sockaddr_in));
    
    while(select(maxfd+1, &read_fds, NULL, NULL, NULL)) {
        if(FD_ISSET(ucast_sockfd,&read_fds)) {
            int bytes_read=recvfrom(ucast_sockfd,buffer,MAX_BUFF_SIZE,0,
                    (struct sockaddr*)&src_addr,&sockaddr_len); // czytam
            if(bytes_read>=0) { // spradzam czy ok
                fprintf(stdout,"UCAST %s:%d: %s",inet_ntoa(src_addr.sin_addr),src_addr.sin_port,buffer);
            } else {    // jakis blad
                fprintf(stderr,"Cos poszlo nie tak,return=%d\n",bytes_read);
            }
        } else if(FD_ISSET(bcast_sockfd,&read_fds)) {
            fprintf(stdout,"Dane przyszły na BROADCAST\n");
            
        } else if(FD_ISSET(mcast_sockfd,&read_fds)) {
            fprintf(stdout,"Dane przyszły na MULTICAST\n");
            
        } else {
            fprintf(stderr,"coś niespodziewanego się stało...\n");
        }
        
        // czyszczenie pamieci
        memset(buffer,0,MAX_BUFF_SIZE);
        
        // ponowne wypełnienie zbioru deskryptorów
        FD_ZERO(&read_fds);
        FD_SET(ucast_sockfd,&read_fds);
        FD_SET(bcast_sockfd,&read_fds);
        FD_SET(mcast_sockfd,&read_fds);
    }
    
    return EXIT_SUCCESS;
}