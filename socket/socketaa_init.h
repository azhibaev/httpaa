/* Socket library. Server and client.
 * Copyright (C) 2021  Askar Azhibaev
 * 
 * File: socketaa_init.h
 * Description: Socket library
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: December 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef SOCKETAA_INIT_H
#define SOCKETAA_INIT_H

#ifdef WIN32
SOCKETAA_INLINE int socketaa_init_wsa(int version, int revision)
{
	WSADATA wsa_data;
	int error = 0;
	
	error = WSAStartup(MAKEWORD(version, revision), &wsa_data);
	if (error != SOCKETAA_WSA_OK)
	{
		return error;
	}
	if (LOBYTE(wsa_data.wVersion) != version || HIBYTE(wsa_data.wVersion) != revision)
	{
		WSACleanup();
		return SOCKETAA_WSA_VERSION_NOT_SUPPORTED;
	}

	return SOCKETAA_WSA_OK;
}
#endif

SOCKETAA_INLINE int socketaa_free_wsa()
{
#ifdef WIN32
	int error = 0;
	
	error = WSAGetLastError();
	WSACleanup();

	return error;
#else
	return 0;
#endif
}

#endif	/* SOCKETAA_INIT_H */

