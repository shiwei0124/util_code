#ifndef _SOCKET_IO_DEFINE_H
#define _SOCKET_IO_DEFINE_H
#include "hlog/hlog.h"
//

#define SOCKET_IO_TCP_RECV_FAILED			0x0001	//recv tcp failed
#define SOCKET_IO_TCP_SEND_FAILED			0x0002  //send tcp failed
#define SOCKET_IO_TCP_CONNECT_FAILED		0x0003	//tcp connect failed

#define SOCKET_IO_UDP_RECV_FAILED			0x0011	//recv udp failed
#define SOCKET_IO_UDP_SEND_FAILED			0x0012  //send udp failed


HLOG_HANLDE_MODULE(SOCKET_IO)

#define SOCKET_IO_FATAL(fmt, ...) \
{\
	hlog_print(HLOG_LEVEL_FATAL, HLOGGER(SOCKET_IO), 0, "", 0, 0, __HLOG_FILE__, __HLOG_FUNC__, __HLOG_LINE__, fmt, ##__VA_ARGS__);\
}

#define SOCKET_IO_ERROR(fmt, ...) \
{\
	hlog_print(HLOG_LEVEL_ERROR, HLOGGER(SOCKET_IO), 0, "", 0, 0, __HLOG_FILE__, __HLOG_FUNC__, __HLOG_LINE__, fmt, ##__VA_ARGS__);\
}

#define SOCKET_IO_WARN(fmt, ...) \
{\
	hlog_print(HLOG_LEVEL_WARN, HLOGGER(SOCKET_IO), 0, "", 0, 0, __HLOG_FILE__, __HLOG_FUNC__, __HLOG_LINE__, fmt, ##__VA_ARGS__);\
}

#define SOCKET_IO_INFO(fmt, ...) \
{\
	hlog_print(HLOG_LEVEL_INFO, HLOGGER(SOCKET_IO), 0, "", 0, 0, __HLOG_FILE__, __HLOG_FUNC__, __HLOG_LINE__, fmt, ##__VA_ARGS__);\
}

#define SOCKET_IO_DEBUG(fmt, ...) \
{\
	hlog_print(HLOG_LEVEL_DEBUG, HLOGGER(SOCKET_IO), 0, "", 0, 0, __HLOG_FILE__, __HLOG_FUNC__, __HLOG_LINE__, fmt, ##__VA_ARGS__);\
}

#define SOCKET_IO_TRACE(fmt, ...) \
{\
	hlog_print(HLOG_LEVEL_TRACE, HLOGGER(SOCKET_IO), 0, "", 0, 0, __HLOG_FILE__, __HLOG_FUNC__, __HLOG_LINE__, fmt, ##__VA_ARGS__);\
}

#endif