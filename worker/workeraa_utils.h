/* Worker library. Pool of pthread workers.
 * Copyright (C) 2021  Askar Azhibaev
 * 
 * File: workeraa_utils.h
 * Description: Worker library
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: February 2021
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef __WORKERAA_UTILS_H
#define __WORKERAA_UTILS_H

#include <stdio.h>
#include <pthread.h>

WORKERAA_INLINE int workeraa_func(workeraa *worker, 
		enum workeraa_func_enum n_func)
{
	int is_func = 0;

	if (worker)
	{
		switch (worker->work_id)
		{
			case WORKERAA_HTTP_PORT:
				is_func = workeraa_http_func(worker, n_func);
				break;
			case WORKERAA_HTTP_DEBUG_PORT:
				is_func = workeraa_http_func(worker, n_func);
				break;
			case WORKERAA_HTTPS_PORT:
				is_func = workeraa_http_func(worker, n_func);
				break;
			case WORKERAA_HTTPS_DEBUG_PORT:
				is_func = workeraa_http_func(worker, n_func);
				break;
			default:
				break;
		}
	}

	return is_func;
}

/* Set work for worker
 * 1. Server gets connection
 * 2. Server has socket s_conn and port
 * 3. Server makes serveraa work_struct from s_conn and port
 * 4. Server calls workeraa_set_work
 * 5. Server continue to listen 
 * 1	if (!pthread_mutex_lock)
 * 1.1		if (check write index)
 * 1.1.1		if (socketaa_set_work(work_write_index, p_work))
 * 1.1.1.1			work_count++
 * 1.1.1.2			if (set_next_write_index)
 * 1.1.1.3			if (!pthread_cond_signal)
 * 1.1.2.1.1				is_set = 1
 * 2	if (!pthread_mutex_unlock)
 */
WORKERAA_INLINE int workeraa_set_work(workeraa_pool *pool,
		void *p_work)
{
	int is_set = 0;

	if (pool)
	{
		if (!pthread_mutex_lock(&pool->sync->mutex))
		{
			/* Add work */
			if (workeraa_work_check_write_index(pool))
			{
				if (socketaa_set_work(pool->sync->work + pool->sync->work_write_index, p_work))
				{
					if (workeraa_work_set_next_write_index(pool))
					{
					}
					pool->sync->work_count++;
					if (pthread_cond_signal(&pool->sync->cond))
					{
						/* error */
					}
					is_set = 1;
				}
			}
			
			if (!pthread_mutex_unlock(&pool->sync->mutex))
			{
			}
		}
	}

	return is_set;
}

/* Get work for workers 
 * 1. Worker waiting for work
 * 2. Worker gets signal of work
 * 3. Worker reads sync->work p_work */
WORKERAA_INLINE int workeraa_get_work(workeraa *worker)
{
	int is_get = 0;

	if (worker)
	{
		if (!pthread_mutex_lock(&worker->sync->mutex))
		{
			/* Waiting for work */
			while (worker->sync->work_count == 0)
			{
				if (pthread_cond_wait(&worker->sync->cond, &worker->sync->mutex))
				{
					/* error */
					break;
				}
			}

			/* Get work */
			if (worker->sync->work_count > 0)
			{
				if (workeraa_work_check_read_index(worker))
				{
					worker->work.p_work = worker->sync->work[worker->sync->work_read_index].p_work;
					if (workeraa_work_set_next_read_index(worker))
					{
					}
					worker->sync->work_count--;
					is_get = 1;
				}
			}

			if (!pthread_mutex_unlock(&worker->sync->mutex))
			{
			}
		}
	}

	return is_get;
}

/* Main function 
 * 1. Call get_work
 * 2. Worker calls solver with work link
 * 3. Solver gets port from work link
 * 4. Solver assigns callback function, for example http_init
 * 5. Worker calls Socket worker with s_conn and recv and send callback for data
 * 6. Socket worker reads or sends data over s_conn and call recv or send callback
 * 7. Recv or send callback process data and return with status
 * 8. Socket worker ends
 * 9. Worker returns to waiting for work */
WORKERAA_INLINE int workeraa_main(workeraa *worker)
{
	int status = 0;

	while (1)
	{
		if (workeraa_get_work(worker))
		{
			if ((worker->work_id = socketaa_get_port(&worker->work)))
			{
				if (workeraa_func(worker, WORKERAA_FUNC_INIT))
				{
					if (socketaa_worker(worker))
					{
					}
				}
			}
		}
	}

	return status;
}

/* Start function for worker running in thread */
WORKERAA_INLINE void* workeraa_start(void *worker)
{
	int ret = 0;

	ret = workeraa_main((workeraa*)worker);

	pthread_exit((void*)&ret);
}

/* Begin new work for free worker */
WORKERAA_INLINE int workeraa_begin(workeraa_pool *pool)
{
	int is_begin = 0;

	return is_begin;
}

/* Run worker in thread */
WORKERAA_INLINE int workeraa_run(workeraa *worker)
{
	int is_run = 0;
	pthread_t *thread = NULL;
	pthread_attr_t *attr = NULL;
	int ret = 0;

	if (worker)
	{
		thread = &worker->thread;
		attr = &worker->attr;

		ret = pthread_attr_init(attr);

		ret = pthread_create(thread, attr, workeraa_start, (void*)worker);
		if (ret)
		{
			/* Error */
			ret = pthread_attr_destroy(attr);
			*thread = 0;
			memset(attr, 0, sizeof(*attr));
			__WORKERAA_DEBUG(fprintf(stderr, "Worker thread %lx create failed.\n", *thread););
		}
		else
		{
			/* Created */
			is_run = 1;
			__WORKERAA_DEBUG(fprintf(stderr, "Worker thread %lx created.\n", *thread););
		}
	}

	return is_run;
}

/* Init workeraa_sync struct */
WORKERAA_INLINE int workeraa_sync_init(workeraa_pool *pool)
{
	int is_init = 0;

	if (pool)
	{
		pool->sync = (workeraa_sync*) malloc(sizeof(workeraa_sync));
		if (pool->sync)
		{
			memset(pool->sync, 0, sizeof(workeraa_sync));
			if (pool->work_size_max == 0)
			{
				pool->work_size_max = WORKERAA_POOL_WORK_COUNT_DEFAULT;
			}
			pool->sync->work = (workeraa_work*) malloc(pool->work_size_max * sizeof(workeraa_work));
			if (pool->sync->work)
			{
				memset(pool->sync->work, 0, pool->work_size_max * sizeof(workeraa_work));
				pool->sync->work_size = pool->work_size_max;
				if (!pthread_mutex_init(&pool->sync->mutex, NULL))
				{
					if (!pthread_cond_init(&pool->sync->cond, NULL))
					{
						is_init = 1;
					}
				}
			}
		}

		if (!is_init)
		{
			free(pool->sync);
			pool->sync = NULL;
		}
	}

	return is_init;
}

/* Init worker in 'worker_count' threads */
WORKERAA_INLINE int workeraa_pool_init(workeraa_pool *pool)
{
	int is_init = 0;
	workeraa *worker = NULL;
	size_t worker_count = 0;
	size_t i = 0;

	if (pool)
	{
		worker_count = pool->worker_count;
		if (worker_count > 0)
		{
			worker = (workeraa*) malloc(worker_count * sizeof(workeraa));
			memset(worker, 0, worker_count * sizeof(workeraa));
		}

		if (worker)
		{
			if (workeraa_sync_init(pool))
			{
				for (i = 0; i < worker_count; i++)
				{
					worker[i].sync = pool->sync;
					if (!workeraa_run((void*)&worker[i]))
					{
						break;
					}
				}
			}
		}

		pool->worker = worker;
		pool->worker_count = i;
	}

	return is_init;
}

#endif	/* __WORKERAA_UTILS_H */

