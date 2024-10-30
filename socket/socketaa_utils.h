/* Socket library. Server and client.
 * Copyright (C) 2021  Askar Azhibaev
 * 
 * File: socketaa_utils.h
 * Description: Socket library
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: December 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef __SOCKETAA_UTILS_H
#define __SOCKETAA_UTILS_H

SOCKETAA_INLINE int socketaa_write_response(workeraa *worker)
{
	int is_write = 0;

	return is_write;
}

SOCKETAA_INLINE unsigned short socketaa_get_port(workeraa_work *work)
{
	unsigned short port = 0;
	socketaa_work *socket_work = NULL;

	if (work)
	{
		if (work->p_work)
		{
			socket_work = (socketaa_work*)work->p_work;
			port = socket_work->port;
		}
	}

	return port;
}

SOCKETAA_INLINE int socketaa_set_work(workeraa_work *worker_work, void *p_work)
{
	int is_set = 0;
	socketaa_work *work = NULL;

	if (worker_work)
	{
		if (worker_work->p_work)
		{
			work = (socketaa_work*)worker_work->p_work;
		}
		else
		{
			work = (socketaa_work*) malloc(sizeof(socketaa_work));
			worker_work->p_work = (void*)work;
		}

		if (work)
		{
			*work = *((socketaa_work*)p_work);
			is_set = 1;
		}
	}

	return is_set;
}

SOCKETAA_INLINE int socketaa_set_work_flags(workeraa_work *worker_work, unsigned short flags)
{
	int is_set = 0;
	socketaa_work *work = NULL;

	if (worker_work)
	{
		if (worker_work->p_work)
		{
			work = (socketaa_work*)worker_work->p_work;
			work->flags = flags;
			is_set = 1;
		}
	}

	return is_set;
}

SOCKETAA_INLINE int socketaa_get_work_flags(workeraa_work *worker_work, unsigned short *flags)
{
	int is_set = 0;
	socketaa_work *work = NULL;

	if (worker_work)
	{
		if (worker_work->p_work)
		{
			work = (socketaa_work*)worker_work->p_work;
			*flags = work->flags;
			is_set = 1;
		}
	}

	return is_set;
}

#endif	/* __SOCKETAA_UTILS_H */

