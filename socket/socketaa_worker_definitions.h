/* Socket library. Server and client.
 * Copyright (C) 2021  Askar Azhibaev
 *
 * File: socketaa_worker_definitions.h
 * Description: Socket library
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: February 2021
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef __SOCKETAA_WORKER_DEFINITIONS_H
#define __SOCKETAA_WORKER_DEFINITIONS_H

#include "socketaa_external_definitions.h"

SOCKETAA_INLINE int socketaa_worker(workeraa *worker);
SOCKETAA_INLINE unsigned short socketaa_get_port(workeraa_work *work);
SOCKETAA_INLINE int socketaa_set_work(workeraa_work *worker_work, void *p_work);
SOCKETAA_INLINE int socketaa_set_work_flags(workeraa_work *worker_work, unsigned short flags);
SOCKETAA_INLINE int socketaa_get_work_flags(workeraa_work *worker_work, unsigned short *flags);

#endif	/* __SOCKETAA_WORKER_DEFINITIONS_H */
