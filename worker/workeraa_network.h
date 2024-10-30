/* Worker library. Pool of pthread workers.
 * Copyright (C) 2021  Askar Azhibaev
 * 
 * File: workeraa_network.h
 * Description: Worker NETWORK
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: February 2021
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef __WORKERAA_NETWORK_H
#define __WORKERAA_NETWORK_H

WORKERAA_INLINE int workeraa_network_init(workeraa *worker);
WORKERAA_INLINE int workeraa_network_func(workeraa *worker, 
		enum workeraa_func_enum n_func);

/* Callback recv for network
 * recv data from worker */
WORKERAA_INLINE int workeraa_network_recv(workeraa *worker)
{
	int is_recv = 0;

	if (worker)
	{
		is_recv = workeraa_network_func(worker, WORKERAA_FUNC_RECV_DATA);
	}

	return is_recv;
}

/* Callback send for network
 * send data to worker */
WORKERAA_INLINE int workeraa_network_send(workeraa *worker)
{
	int is_send = 0;

	if (worker)
	{
		is_send = workeraa_network_func(worker, WORKERAA_FUNC_SEND_DATA);
	}

	return is_send;
}

/* Free network worker struct link */
WORKERAA_INLINE int workeraa_network_free(workeraa *worker)
{
	int is_free = 0;

	if (worker)
	{
		if (worker->work.p_struct)
		{
			free((workeraa*)worker->work.p_struct);
			worker->work.p_struct = NULL;
		}
		is_free = 1;
	}

	return is_free;
}

/* Init network worker link */
WORKERAA_INLINE int workeraa_network_init(workeraa *worker)
{
	int is_init = 0;

	if (worker)
	{
		if (worker->work.p_struct == NULL)
		{
			worker->work.p_struct = (void*) malloc(sizeof(workeraa));
		}
		is_init = 1;
	}

	return is_init;
}

WORKERAA_INLINE int workeraa_network_func(workeraa *worker, 
		enum workeraa_func_enum n_func)
{
	int is_func = 0;

	switch (n_func)
	{
		case WORKERAA_FUNC_INIT:
			is_func = workeraa_network_init(worker);
			break;
		case WORKERAA_FUNC_RECV_DATA:
			is_func = workeraa_network_recv(worker);
			break;
		case WORKERAA_FUNC_SEND_DATA:
			is_func = workeraa_network_send(worker);
			break;
		case WORKERAA_FUNC_FREE:
			is_func = workeraa_network_free(worker);
			break;
		default:
			break;
	}

	return is_func;
}

#endif	// __WORKERAA_NETWORK_H

