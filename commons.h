/* 
 * File:   commons.h
 * Author: pawtom
 *
 * Created on 9 grudzień 2015, 11:36
 */

#ifndef COMMONS_H
#define	COMMONS_H

#include <inttypes.h>

#define MAX_DATASIZE_BYTES 1024
#define UNICAST_PORT 31337
#define BROADCAST_PORT 31338
#define MULTICAST_PORT 31339

typedef struct {
    uint32_t id;
    uint32_t src;
    uint32_t dst;
    uint32_t size;
} Header;

typedef struct {
    Header header;
    uint8_t data[MAX_DATASIZE_BYTES];
} Datagram;

#endif	/* COMMONS_H */

