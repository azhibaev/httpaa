/* Socket library. Server and client.
 * Copyright (C) 2021  Askar Azhibaev
 * 
 * File: socketaa_server.h
 * Description: Socket library
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: December 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef __SOCKETAA_SERVER_H
#define __SOCKETAA_SERVER_H

/* Main function for server */
SOCKETAA_INLINE int socketaa_server(socketaa *server)
{
	int is_wsa = SOCKETAA_WSA_OK;
	int error = 0;
	unsigned long argp = 0;
	struct sockaddr_in addr = {0};
	struct sockaddr_in addr_conn = {0};
	unsigned int addr_conn_size = sizeof(addr_conn);
	int sock_count = 0;
	int is_select = 1;
	int is_read = 1;		/* accept */
	int is_write = 0;
	int is_accepted = 0;
	SOCKET s_conn = 0;
	int max_socket = 0;	/* Linux */
	fd_set sock_set_read;
	fd_set sock_set_write;
	char *s_addr_conn = NULL;
	unsigned short port = 0;
	int is_enable = 1;

	if (server)
	{

		port = server->work.port;
		if (server->work.pool.worker_count == 0)
		{
			server->work.pool.worker_count = WORKERAA_POOL_WORKER_COUNT_DEFAULT;
		}
		else if (server->work.pool.worker_count > WORKERAA_POOL_WORKER_COUNT_MAX)
		{
			server->work.pool.worker_count = WORKERAA_POOL_WORKER_COUNT_MAX;
		}
		if (workeraa_pool_init(&server->work.pool))
		{
		}

#ifdef WIN32
		is_wsa = socketaa_init_wsa(2, 2);
#endif
		if (is_wsa == SOCKETAA_WSA_OK)
		{
			error = 0;
			SOCKET s1 = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (s1 == (SOCKET)(~0))
			{
				__SOCKETAA_DEBUG(fprintf(stderr, "Create socket: error\n"););
				error = socketaa_free_wsa();
				return error;
			}
#ifdef WIN32
			argp = 1;
			error = ioctlsocket(s1, FIONBIO, &argp);
#else												/* Linux */
			argp = 0;
			error = fcntl(s1, F_GETFL, &argp);
			error = fcntl(s1, F_SETFL, argp | O_NONBLOCK);
#endif
			if (error != SOCKETAA_WSA_OK)
			{
				__SOCKETAA_DEBUG(fprintf(stderr, "Set socket O_NONBLOCK: error\n"););
				error = socketaa_free_wsa();
				return error;
			}
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.s_addr = htonl(INADDR_ANY);
			if (setsockopt(s1, SOL_SOCKET, SO_REUSEADDR, &is_enable, sizeof(int)))
			{
				__SOCKETAA_DEBUG(fprintf(stderr, "Set socket option SO_REUSEADDR: error\n"););
			}
			error = bind(s1, (struct sockaddr*)&addr, sizeof(addr));
			if (error != SOCKETAA_WSA_OK)
			{
				perror("Bind socket error");
				__SOCKETAA_DEBUG(fprintf(stderr, "Bind socket: error\n"););
				error = socketaa_free_wsa();
				return error;
			}
			error = listen(s1, SOMAXCONN);
			if (error != SOCKETAA_WSA_OK)
			{
				__SOCKETAA_DEBUG(fprintf(stderr, "Listen socket: error\n"););
				error = socketaa_free_wsa();
				return error;
			}
			__SOCKETAA_DEBUG(fprintf(stderr, "listen at port: %d\n", port););
			addr_conn_size = sizeof(addr_conn);
			FD_ZERO(&sock_set_read);
			FD_ZERO(&sock_set_write);
			max_socket = s1 + 1;	/* Linux */
			while (is_select)
			{
				FD_ZERO(&sock_set_read);
				FD_ZERO(&sock_set_write);
				if (is_read)
				{
					FD_SET(s1, &sock_set_read);
					is_read = 0;
					__SOCKETAA_DEBUG(fprintf(stderr, "Set read\n"););
				}
				if (is_write)
				{
					FD_SET(s1, &sock_set_write);
					is_write = 0;
					__SOCKETAA_DEBUG(fprintf(stderr, "Set write\n"););
				}
				sock_count = select(max_socket, &sock_set_read, &sock_set_write, NULL, NULL);
				__SOCKETAA_DEBUG(fprintf(stderr, "select\n"););
				if (sock_count > 0)
				{
					if (FD_ISSET(s1, &sock_set_read))
					{
						__SOCKETAA_DEBUG(fprintf(stderr, "Recv...\n"););
						if (is_accepted == 0)
						{
							s_conn = accept(s1, (struct sockaddr *)&addr_conn, &addr_conn_size);
							if (s_conn == (SOCKET)(~0))
							{
								__SOCKETAA_DEBUG(fprintf(stderr, "Connect socket: error\n"););
								error = socketaa_free_wsa();
								return error;
							}
							s_addr_conn = inet_ntoa(addr_conn.sin_addr);
							if (s_addr_conn)
							{
								__SOCKETAA_DEBUG(fprintf(stderr, "connect from %s port %d\n", s_addr_conn, ntohs(addr_conn.sin_port)););
							}
							is_read = 1;
							server->work.conn = s_conn;
							server->work.addr = s_addr_conn;
							server->work.port = port;
							server->work.flags = SOCKETAA_WORK_FLAGS_IS_SELECT | SOCKETAA_WORK_FLAGS_IS_RECV;
							__SOCKETAA_DEBUG(fprintf(stderr, "run client worker\n"););
							/* Server calls Worker
							 * with workeraa_pool and socketaa_work
							 */
							if (workeraa_set_work(&server->work.pool, (void*)&server->work))
							{
							}
						}
					}
				}
				else if (sock_count == 0)
				{
					__SOCKETAA_DEBUG(fprintf(stderr, "select 0\n"););
				}
				else if (sock_count == SOCKETAA_SOCKET_ERROR)
				{
					__SOCKETAA_DEBUG(fprintf(stderr, "select error\n"););
					is_select = 0;
				}
			}
			error = socketaa_free_wsa();
			return error;
		}
	}
	return 0;
}

#endif	/* __SOCKETAA_SERVER_H */

