/* Socket library. Server and client.
 * Copyright (C) 2021  Askar Azhibaev
 * 
 * File: test.c
 * Description: Socket library
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: December 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#include <stdio.h>
#include <limits.h>

#include "socketaa.h"

int test_recv(socketaa_work *work);
int test_send(socketaa_work *work);

int main(int argc, const char* argv[])
{
	socketaa server = {{{0}}};
	char *buf = NULL;
	size_t buf_size = PATH_MAX;

	buf = (char*) malloc(buf_size + 1);
	if (!buf)
	{
		fprintf(stderr, "Error: can not allocate memory\n");
		return -1;
	}
	if (!getcwd(buf, buf_size))
	{
		fprintf(stderr, "Error: can not get current working directory\n");
		return -1;
	}
	if (stringaa_set_chars(&server.work.path, buf))
	{
		server.work.port = WORKERAA_HTTP_DEBUG_PORT;
		server.work.pool.worker_count = WORKERAA_POOL_WORKER_COUNT_MIN + 1;
		server.work.pool.work_size_max = WORKERAA_POOL_WORK_COUNT_MAX;

		socketaa_server(&server);
	}

	free(buf);

	return 0;
}

int test_recv(socketaa_work *work)
{
	int is_test_recv = 0;

	return is_test_recv;
}

int test_send(socketaa_work *work)
{
	int is_test_send = 0;

	return is_test_send;
}

