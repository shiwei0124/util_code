#ifndef _SOCKET_IO_DEFINE_H
#define _SOCKET_IO_DEFINE_H
//
#include "stdio.h"

#define SOCKET_IO_TCP_RECV_FAILED			0x0001	//recv tcp failed
#define SOCKET_IO_TCP_SEND_FAILED			0x0002  //send tcp failed
#define SOCKET_IO_TCP_CONNECT_FAILED		0x0003	//tcp connect failed

#define SOCKET_IO_UDP_RECV_FAILED			0x0011	//recv udp failed
#define SOCKET_IO_UDP_SEND_FAILED			0x0012  //send udp failed


/*
HLOG_HANLDE_MODULE(SOCKET_IO)
*/

#define SOCKET_IO_FATAL(fmt, ...) \
    {\
        printf(fmt, ##__VA_ARGS__);\
        printf("\n");\
    }

#define SOCKET_IO_ERROR(fmt, ...) \
    {\
        printf(fmt, ##__VA_ARGS__);\
        printf("\n");\
    }

#define SOCKET_IO_WARN(fmt, ...) \
    {\
        printf(fmt, ##__VA_ARGS__);\
        printf("\n");\
    }

#define SOCKET_IO_INFO(fmt, ...) \
    {\
        printf(fmt, ##__VA_ARGS__);\
        printf("\n");\
    }

#define SOCKET_IO_DEBUG(fmt, ...) \
    {\
        printf(fmt, ##__VA_ARGS__);\
        printf("\n");\
    }

#define SOCKET_IO_TRACE(fmt, ...) \
    {\
        printf(fmt, ##__VA_ARGS__);\
        printf("\n");\
    }

#endif