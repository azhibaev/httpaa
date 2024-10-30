/* Socket library. Server and client.
 * Copyright (C) 2021  Askar Azhibaev
 * 
 * File: socketaa_debug.h
 * Description: Socket library
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: February 2021
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef __SOCKETAA_DEBUG_H
#define __SOCKETAA_DEBUG_H

SOCKETAA_INLINE int socketaa_print_time()
{
	time_t t = 0;
	time(&t);
	printf("%s|-", ctime(&t));
	return 0;
}

#ifdef _DEBUG
#define __SOCKETAA_DEBUG(p) {socketaa_print_time();p}
#else
#define __SOCKETAA_DEBUG(p)
#endif	/* _DEBUG */

#endif	/* __SOCKETAA_DEBUG_H */
