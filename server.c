#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/param.h>

int init_unicast_socket(int bind_port) {
    int sockfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    
    
    return sockfd;
}

int init_broadcast_socket(int bind_port) {
    int sockfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    
    
    return sockfd;
}

int init_multicast_socket(int bind_port) {
    int sockfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    
    
    return sockfd;
}

int main(void) {
    int ucast_sockfd=init_unicast_socket(31337);
    int bcast_sockfd=init_broadcast_socket(31338);
    int mcast_sockfd=init_multicast_socket(31339);
    
    fprintf(stdout,"Unicast sockfd: %d\nBroadcast sockfd: %d\nMulticast sockfd: %d\n", 
            ucast_sockfd,bcast_sockfd,mcast_sockfd);
    
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(ucast_sockfd,&read_fds);
    FD_SET(bcast_sockfd,&read_fds);
    FD_SET(mcast_sockfd,&read_fds);
    
    int maxfd=MAX(MAX(ucast_sockfd,bcast_sockfd),mcast_sockfd);
    
    while(select(maxfd+1, &read_fds, NULL, NULL, NULL)) {
        
    }
    
    return EXIT_SUCCESS;
}