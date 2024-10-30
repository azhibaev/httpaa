/* Worker library. Pool of pthread workers.
 * Copyright (C) 2021  Askar Azhibaev
 * 
 * File: workeraa_definitions.h
 * Description: Worker library
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: February 2021
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef __WORKERAA_DEFINITIONS_H
#define __WORKERAA_DEFINITIONS_H

#define WORKERAA_INLINE static inline

#ifdef _DEBUG
#define __WORKERAA_DEBUG(p) {p}
#else
#define __WORKERAA_DEBUG(p)
#endif

#ifndef KB
#define KB * (1 << 10)
#define MB KB KB
#endif

enum workeraa_work_id
{
	WORKERAA_HTTP_PORT = 80,
	WORKERAA_HTTP_DEBUG_PORT = 8080,
	WORKERAA_HTTPS_PORT = 443,
	WORKERAA_HTTPS_DEBUG_PORT = 8443
};

enum workeraa_work_enum
{
	WORKERAA_WORK_RECV_BUFFER_SIZE_DEFAULT = 1 MB,
	WORKERAA_WORK_SEND_BUFFER_SIZE_DEFAULT = 1 MB
};

enum workeraa_func_enum
{
	WORKERAA_FUNC_INIT,
	WORKERAA_FUNC_RECV_DATA,
	WORKERAA_FUNC_SEND_DATA,
	WORKERAA_FUNC_GET_RECV_BUFFER,
	WORKERAA_FUNC_GET_SEND_BUFFER,
	WORKERAA_FUNC_GET_BUFFER,
	WORKERAA_FUNC_FREE
};

enum workeraa_pool_enum
{
	WORKERAA_POOL_WORKER_COUNT_MIN = 1,
	WORKERAA_POOL_WORKER_COUNT_MAX = 255,
	WORKERAA_POOL_WORKER_COUNT_DEFAULT = WORKERAA_POOL_WORKER_COUNT_MIN,
	WORKERAA_POOL_WORK_COUNT_MIN = 1,
	WORKERAA_POOL_WORK_COUNT_MAX = 1024,
	WORKERAA_POOL_WORK_COUNT_DEFAULT = WORKERAA_POOL_WORK_COUNT_MAX
};

#endif	/* __WORKERAA_DEFINITIONS_H */

