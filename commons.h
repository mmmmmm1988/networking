/* 
 * File:   commons.h
 * Author: pawtom
 *
 * Created on 9 grudzień 2015, 11:36
 */

#ifndef COMMONS_H
#define	COMMONS_H

#include <inttypes.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/select.h>

#define MAX_BUFF_SIZE 64

#define UNICAST_IP "127.0.0.1"
#define BROADCAST_IP "192.168.255.255"
#define PORT 31337

#define MULTICAST_IP "224.0.0.1"
#define MULTICAST_PORT 31338


//typedef struct {
//    uint32_t id;
//    uint32_t src;
//    uint32_t dst;
//    uint32_t size;
//} Header;

//typedef struct {
//    Header header;
//    #define MAX_DATASIZE_BYTES 1024
//    uint8_t data[MAX_DATASIZE_BYTES];
//} Datagram;

#endif	/* COMMONS_H */

