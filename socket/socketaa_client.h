/* Socket library. Server and client.
 * Copyright (C) 2021  Askar Azhibaev
 *
 * File: socketaa_client.h
 * Description: Socket library
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: December 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef __SOCKETAA_CLIENT_H
#define __SOCKETAA_CLIENT_H

/* Main function for client */
SOCKETAA_INLINE int socketaa_client(socketaa *client)
{
	int sock_count = 0;
	int is_select = 1;
	int is_recv = 0;
	int is_send = 1;		/* send request */
	int is_connected = 0;
	int max_socket = 1;		/* Linux */
	fd_set sock_set_read;
	fd_set sock_set_write;
	struct sockaddr_in addr = {0};
	int error = 0;
	unsigned long argp = 0;

#ifdef WIN32
	int is_wsa = socketaa_init_wsa(2, 2);
#else							/* Linux */
	int is_wsa = SOCKETAA_WSA_OK;
#endif
	if (is_wsa == SOCKETAA_WSA_OK)
	{
		error = 0;
		SOCKET s1 = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (s1 == (SOCKET)(~0))
		{
			error = socketaa_free_wsa();
			return error;
		}
		addr.sin_family = AF_INET;
		addr.sin_port = htons(8080);
		addr.sin_addr.s_addr = inet_addr(client->work.ip);
		error = connect(s1, (struct sockaddr*)&addr, sizeof(addr));
		if (error != SOCKETAA_WSA_OK)
		{
#ifdef WIN32
			error = WSAGetLastError();
			if (error == WSAEWOULDBLOCK)
#else												/* Linux */
			if (error == SOCKETAA_EINPROGRESS)
#endif
			{
				__SOCKETAA_DEBUG(printf("non-blocking connect\n"););
			}
			else
			{
				error = socketaa_free_wsa();
				return error;
			}
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
			error = socketaa_free_wsa();
			return error;
		}
		FD_ZERO(&sock_set_read);
		FD_ZERO(&sock_set_write);
		max_socket = s1 + 1;	/* Linux */
		while (is_select)
		{
			FD_ZERO(&sock_set_read);
			FD_ZERO(&sock_set_write);
			if (is_recv)
			{
				FD_SET(s1, &sock_set_read);
				is_recv = 0;
			}
			if (is_send)
			{
				FD_SET(s1, &sock_set_write);
				is_send = 0;
			}
			sock_count = select(max_socket, &sock_set_read, &sock_set_write, NULL, NULL);
			__SOCKETAA_DEBUG(printf("select\n"););
			if (sock_count > 0)
			{
				if (FD_ISSET(s1, &sock_set_write))
				{
					if (is_connected == 0)
					{
						is_connected = 1;
						is_send = 1;
						is_select = 0;
					}
				}
			}
			else if (sock_count == 0)
			{
				__SOCKETAA_DEBUG(printf("select 0\n"););
			}
			else if (sock_count == SOCKETAA_SOCKET_ERROR)
			{
				__SOCKETAA_DEBUG(printf("select error\n"););
				is_select = 0;
			}
		}
		error = socketaa_free_wsa();
		return error;
	}
	return 0;
}

#endif	/* __SOCKETAA_CLIENT_H */

