/* Socket library. Server and client.
 * Copyright (C) 2021  Askar Azhibaev
 * 
 * File: socketaa_structs.h
 * Description: Socket library
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: February 2021
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef __SOCKETAA_STRUCTS_H
#define __SOCKETAA_STRUCTS_H

struct socket_buffer_struct
{
	stringaa *p_recv;
	stringaa *p_send;
};

struct socket_work_struct
{
	workeraa_pool pool;
	socketaa_buffer buffer;
	stringaa path;
	SOCKET conn;
	char *addr;
	char *ip;
	unsigned short port;
	union
	{
		unsigned short flags;
		struct
		{
			unsigned short is_select : 1;
			unsigned short is_recv : 1;
			unsigned short is_send : 1;
			unsigned short is_recv_shutdown : 1;
			unsigned short is_send_shutdown : 1;
			unsigned short is_receive : 1;
			unsigned short is_init : 1;
		};
	};
};

struct socket_struct
{
	socketaa_work work;
};

#endif	/* __SOCKETAA_STRUCTS_H */

