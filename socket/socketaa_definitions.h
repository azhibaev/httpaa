/* Socket library. Server and client.
 * Copyright (C) 2021  Askar Azhibaev
 * 
 * File: socketaa_definitions.h
 * Description: Socket library
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: February 2021
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef __SOCKETAA_DEFINITIONS_H
#define __SOCKETAA_DEFINITIONS_H

#include <stdio.h>

#ifdef WIN32
#include <winsock2.h>
#else							/* Linux */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#include <time.h>

#include "httpaa.h"

#ifndef SOCKETAA_INLINE
#define SOCKETAA_INLINE static inline
#endif

#ifdef WIN32
#define SOCKETAA_SHUTDOWN_WRITE SD_SEND
#define SOCKETAA_EINPROGRESS EINPROGRESS
#else									/* Linux */
#define SOCKETAA_SOCKET_ERROR -1
#define closesocket close
#define SOCKETAA_SHUTDOWN_WRITE SHUT_WR
#define SOCKETAA_EINPROGRESS 10036
#endif	/* WIN32 */

#define SOCKETAA_WSA_OK 0
#define SOCKETAA_WSA_ERROR -1
#define SOCKETAA_WSA_VERSION_NOT_SUPPORTED SOCKETAA_WSA_ERROR
#define KB * (1 << 10)
#define SOCKETAA_TEST_PORT 8080
#define SOCKETAA_RECV_BUFFER_SIZE 2 KB
#define SOCKETAA_SEND_BUFFER_SIZE 2 KB
#define SOCKETAA_OK 0
#define SOCKETAA_ERROR -1
#define SOCKETAA_ERROR_RECV_SHUTDOWN SOCKETAA_ERROR-1
#define SOCKETAA_ERROR_SEND_SHUTDOWN SOCKETAA_ERROR-2
#define SOCKETAA_WORKER_SELECT_WAIT_SECONDS 3

#ifndef WIN32			/* Linux */
typedef int SOCKET;
#endif

typedef struct socket_struct socketaa;
typedef struct socket_work_struct socketaa_work;
typedef struct socket_buffer_struct socketaa_buffer;
typedef int (*socketaa_work_callback) (workeraa_work*, void*);

#endif	/* __SOCKETAA_DEFINITIONS_H */
