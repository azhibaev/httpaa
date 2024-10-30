/* Worker library. Pool of pthread workers.
 * Copyright (C) 2021  Askar Azhibaev
 * 
 * File: workeraa_structs.h
 * Description: Worker library
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: February 2021
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef __WORKERAA_STRUCTS_H
#define __WORKERAA_STRUCTS_H

typedef struct worker_struct workeraa;
typedef struct worker_sync workeraa_sync;
typedef struct worker_pool workeraa_pool;
typedef struct worker_work workeraa_work;

struct worker_work
{
	void *p_work;					/* pointer to external work */
	void *p_struct;					/* pointer to external work struct */
	void *p_data;					/* pointer to external data */
};

struct worker_struct				/* worker struct */
{	
	pthread_t thread;				/* thread of worker */
	pthread_attr_t attr;			/* attr of thread */
	workeraa_sync *sync;			/* sync struct */
	workeraa_work work;				/* pointer to work array */
	enum workeraa_work_id work_id;	/* work id or socket port */
};

struct worker_pool					/* pool of workers */
{
	workeraa *worker;				/* pointer to array of workers */
	size_t worker_count;			/* count of workers */
	workeraa_sync *sync;			/* sync struct */
	size_t work_size_max;			/* max size of works array */
};

struct worker_sync					/* sync struct */
{
	pthread_mutex_t mutex;			/* sync write */
	pthread_cond_t cond;			/* sync signal */
	workeraa_work *work;			/* work link */
	size_t work_count;				/* count of available works */
	size_t work_read_index;			/* read index of work */
	size_t work_write_index;		/* write index of work */
	size_t work_size;				/* size of works array */
};

#endif	/* __WORKERAA_STRUCTS_H */

