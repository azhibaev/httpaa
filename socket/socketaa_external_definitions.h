/* Socket library. Server and client.
 * Copyright (C) 2021  Askar Azhibaev
 * 
 * File: socketaa_external_definitions.h
 * Description: Socket library
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: February 2021
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef __SOCKETAA_EXTERNAL_DEFINITIONS_H
#define __SOCKETAA_EXTERNAL_DEFINITIONS_H

#ifndef SOCKETAA_INLINE
#define SOCKETAA_INLINE static inline
#endif

#ifndef __SOCKETAA_WORK_FLAGS_IS_DEFINED
#define __SOCKETAA_WORK_FLAGS_IS_DEFINED
enum socketaa_work_flags
{
	SOCKETAA_WORK_FLAGS_IS_SELECT = 1 << 0,
	SOCKETAA_WORK_FLAGS_IS_RECV = 1 << 1,
	SOCKETAA_WORK_FLAGS_IS_SEND = 1 << 2,
	SOCKETAA_WORK_FLAGS_IS_RECV_SHUTDOWN = 1 << 3,
	SOCKETAA_WORK_FLAGS_IS_SEND_SHUTDOWN = 1 << 4
};
#endif	/* __SOCKETAA_WORK_FLAGS_IS_DEFINED */

#endif	/* __SOCKETAA_EXTERNAL_DEFINITIONS_H */
