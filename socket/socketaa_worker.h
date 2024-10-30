/* Socket library. Server and client.
 * Copyright (C) 2021  Askar Azhibaev
 * 
 * File: socketaa_worker.h
 * Description: Socket library
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: December 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef __SOCKETAA_WORKER_H
#define __SOCKETAA_WORKER_H

#include <time.h>

/* Receive bytes_recv
 * to s_recv->buf + s_recv->write_pos
 */
SOCKETAA_INLINE ssize_t socketaa_worker_recv(SOCKET s_conn, stringaa *s_recv)
{
	ssize_t bytes_recv = 0;

	if (s_recv->write_pos < s_recv->buf_size)
	{
		bytes_recv = recv(s_conn, s_recv->buf + s_recv->write_pos, s_recv->buf_size - s_recv->write_pos, 0);
		if (bytes_recv == 0)
		{
			/* recv returns 0 if TCP socket recv shutdown */
			bytes_recv = SOCKETAA_ERROR_RECV_SHUTDOWN;
		}
		else if (bytes_recv > 0)
		{
			s_recv->write_pos += bytes_recv;
		}
	}

	return bytes_recv;
}

/* Send bytes_send
 * from s_send->buf + s_send->read_pos
 */
SOCKETAA_INLINE ssize_t socketaa_worker_send(SOCKET s_conn, stringaa *s_send)
{
	ssize_t bytes_send = 0;

	if (s_send->write_pos < s_send->buf_size
			&& s_send->read_pos < s_send->write_pos)
	{
		bytes_send = send(s_conn, s_send->buf + s_send->read_pos, s_send->write_pos - s_send->read_pos, 0);
		if (bytes_send == 0)
		{
			/* send returns 0 if TCP socket send shutdown */
			bytes_send = SOCKETAA_ERROR_SEND_SHUTDOWN;
		}
		else if (bytes_send > 0)
		{
			s_send->read_pos += bytes_send;
		}
	}

	return bytes_send;
}

SOCKETAA_INLINE int socketaa_worker_check_recv(workeraa *worker, socketaa_work *work, SOCKET s_conn, int bytes_recv)
{
	int is_check = 0;
	int is_func = 0;

	if (worker && work)
	{
		is_check = 1;
		if (bytes_recv > 0)
		{
			__SOCKETAA_DEBUG(fprintf(stderr, "recv %d bytes\n", bytes_recv););
			work->is_recv = 1;
			/* If SOCKETAA_OK, then recv
			 * If SOCKETAA_ERROR, then error
			 */
			is_func = workeraa_func(worker, WORKERAA_FUNC_RECV_DATA);
			if (is_func == SOCKETAA_ERROR)
			{
				/* Error
				 * close socket
				 */
				__SOCKETAA_DEBUG(fprintf(stderr, "parse error\n"););
				shutdown(s_conn, SOCKETAA_SHUTDOWN_WRITE);
				closesocket(s_conn);
				work->is_select = 0;
			}
		}
		/* No free space in recv buffer */
		else if (bytes_recv == 0)
		{
			/* Close socket	*/
			__SOCKETAA_DEBUG(fprintf(stderr, "no free space in recv buffer\n"););
			shutdown(s_conn, SOCKETAA_SHUTDOWN_WRITE);
			closesocket(s_conn);
			work->is_select = 0;
		}
		/* Error of recv */
		else if (bytes_recv == SOCKETAA_SOCKET_ERROR)
		{
			/* Close socket	*/
			__SOCKETAA_DEBUG(fprintf(stderr, "recv error / connection closed\n"););
			closesocket(s_conn);
			work->is_select = 0;
		}
		/* Receive recv shutdown */
		else if (bytes_recv == SOCKETAA_ERROR_RECV_SHUTDOWN)
		{
			__SOCKETAA_DEBUG(fprintf(stderr, "receive recv shutdown\n"););
			work->is_recv_shutdown = 1;
		}
	}

	return is_check;
}

SOCKETAA_INLINE int socketaa_worker_check_send(socketaa_work *work, SOCKET s_conn, int bytes_send)
{
	int is_check = 0;

	if (work)
	{
		is_check = 1;
		if (bytes_send > 0)
		{
			__SOCKETAA_DEBUG(fprintf(stderr, "send %d bytes\n", bytes_send););
			work->is_send = 1;
			/* If SOCKETAA_OK, then send
			 * If SOCKETAA_ERROR, then error
			 */
		}
		/* No free space in send buffer */
		else if (bytes_send == 0)
		{
			/* Close socket	*/
			__SOCKETAA_DEBUG(fprintf(stderr, "no free space in send buffer\n"););
			shutdown(s_conn, SOCKETAA_SHUTDOWN_WRITE);
			closesocket(s_conn);
			work->is_select = 0;
		}
		/* Error of send */
		else if (bytes_send == SOCKETAA_SOCKET_ERROR)
		{
			/* Close socket	*/
			__SOCKETAA_DEBUG(fprintf(stderr, "send error / connection closed\n"););
			closesocket(s_conn);
			work->is_select = 0;
		}
		/* Receive send shutdown */
		else if (bytes_send == SOCKETAA_ERROR_RECV_SHUTDOWN)
		{
			__SOCKETAA_DEBUG(fprintf(stderr, "receive send shutdown\n"););
			work->is_send_shutdown = 1;
		}

		if (bytes_send != SOCKETAA_SOCKET_ERROR)
		{
			__SOCKETAA_DEBUG(fprintf(stderr, "send error\n"););
			work->is_send = 1;
		}
		else
		{
			__SOCKETAA_DEBUG(fprintf(stderr, "send error / connection closed\n"););
			shutdown(s_conn, SOCKETAA_SHUTDOWN_WRITE);
			work->is_recv = 1;
		}
	}

	return is_check;
}

SOCKETAA_INLINE int socketaa_worker(workeraa *worker) 
{
	/* define variables */
	int sock_count = 0;
	int max_socket = 0;
	int bytes_recv = 0;
	int bytes_send = 0;
	int is_init = 0;
	struct timeval socket_wait = {0};
	SOCKET s_conn = 0;
	fd_set sock_set_recv;
	fd_set sock_set_send;
	stringaa *p_recv = NULL;
	stringaa *p_send = NULL;
	socketaa_work *work = NULL;
	int is_func = 0;

	/* define structs */
	if (worker)
	{
		work = (socketaa_work*)worker->work.p_work;
		if (work)
		{
			if (workeraa_func(worker, WORKERAA_FUNC_GET_RECV_BUFFER))
			{
				if (worker->work.p_data)
				{
					p_recv = worker->work.p_data;
					if (workeraa_func(worker, WORKERAA_FUNC_GET_SEND_BUFFER))
					{
						p_send = worker->work.p_data;
						is_init = 1;
					}
				}
			}
		}
	}
	if (!is_init)
	{
		return 0;
	}

	s_conn = work->conn;
	worker->work_id = (unsigned int)work->port;
	max_socket = s_conn + 1;    /* Linux */
	FD_ZERO(&sock_set_recv);
	FD_ZERO(&sock_set_send);
	socket_wait.tv_sec = SOCKETAA_WORKER_SELECT_WAIT_SECONDS;
	socket_wait.tv_usec = 0;
	while (work->is_select)
	{
		FD_ZERO(&sock_set_recv);
		FD_ZERO(&sock_set_send);
		if (work->is_send)
		{
			/* Send	*/
			__SOCKETAA_DEBUG(fprintf(stderr, "send..\n"););
			FD_SET(s_conn, &sock_set_send);
			work->is_send = 0;
		}
		else if (work->is_recv)
		{
			/* Recv	*/
			__SOCKETAA_DEBUG(fprintf(stderr, "recv..\n"););
			FD_SET(s_conn, &sock_set_recv);
			work->is_recv = 0;
		}
		sock_count = select(max_socket, &sock_set_recv, &sock_set_send, NULL, &socket_wait);
		__SOCKETAA_DEBUG(fprintf(stderr, "select: %d\n", sock_count););
		if (sock_count > 0)
		{
			/* Select recv */
			if (FD_ISSET(s_conn, &sock_set_recv))
			{
				bytes_recv = socketaa_worker_recv(s_conn, p_recv);
				/* Receive > 0 bytes */
				socketaa_worker_check_recv(worker, work, s_conn, bytes_recv);
			}
			/* Select send */
			if (FD_ISSET(s_conn, &sock_set_send))
			{
				is_func = workeraa_func(worker, WORKERAA_FUNC_SEND_DATA);
				if (is_func == SOCKETAA_ERROR)
				{
					/* Error
					 * close socket
					 */
					__SOCKETAA_DEBUG(fprintf(stderr, "parse error\n"););
					shutdown(s_conn, SOCKETAA_SHUTDOWN_WRITE);
					closesocket(s_conn);
					work->is_select = 0;
				}
				/* Send write_pos - read_pos bytes from buf + read_pos
				*/
				bytes_send = socketaa_worker_send(s_conn, p_send);
				socketaa_worker_check_send(work, s_conn, bytes_send);
			}
		}
		else if (sock_count == 0)
		{
			/* Timeout */
			__SOCKETAA_DEBUG(fprintf(stderr, "select 0: timeout\n"););
			closesocket(s_conn);
			work->is_select = 0;
		}
		else if (sock_count == SOCKETAA_SOCKET_ERROR)
		{
			__SOCKETAA_DEBUG(fprintf(stderr, "select error\n"););
			work->is_select = 0;
		}
	}
	__SOCKETAA_DEBUG(fprintf(stderr, "worker quit\n"););
	return 0;
}

#endif	/* __SOCKETAA_WORKER_H */

