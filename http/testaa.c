/* HTTP/1.1 RFC 2616
 * Copyright (C) 2021  Askar Azhibaev
 *
 * File: testaa.c
 * Description: Test program
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: December 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "httpaa.h"

int print_url(uriaa *uri);

int main(int argc, const char *argv[])
{
	httpaa http = {{{0}}};
	stringaa request = {0};
	int i = 0;
	int index = 0;
	const char *s_header = NULL;

	memset(&http, 0, sizeof(http));

	stringaa_set_chars(&request, "GET http://localhost:8080/ HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: Mozilla/5.0 (compatible;)\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\nConnection: keep-alive\r\nCache-Control: max-age=0\r\n\r\nMessage");

	http11_request(&http, &request);

	printf("Method: %s\n", http11_method_array[http.request.request_line.method]);
	printf("URL: ");
	stringaa_print_line(&http.request.request_line.request_uri.uri);
	printf("HTTP version: %d.%d\n", http.request.request_line.http_version.major_version, http.request.request_line.http_version.minor_version);
	for (i = HTTP11_GENERAL_HEADER_ARRAY_START; i < HTTP11_GENERAL_HEADER_ARRAY_SIZE + HTTP11_REQUEST_HEADER_ARRAY_SIZE + HTTP11_ENTITY_HEADER_ARRAY_SIZE; i++)
	{
		if (http.request.header[i].is_init)
		{
			if (i < HTTP11_ENTITY_HEADER_ARRAY_START)
			{
				index = i;
				s_header = http11_general_header_original_array[index];
			}
			else if (i < HTTP11_REQUEST_HEADER_ARRAY_START)
			{
				index = i - HTTP11_ENTITY_HEADER_ARRAY_START;
				s_header = http11_entity_header_original_array[index];
			}
			else
			{
				index = i - HTTP11_REQUEST_HEADER_ARRAY_START;
				s_header = http11_request_header_original_array[index];
			}
			printf("%s (%d)", s_header, i);
			stringaa_print(&http.request.header[i]);
			printf("\n");
		}
	}

	print_url(&http.request.request_line.request_uri);

	return 0;
}

int print_url(uriaa *uri)
{
		printf("\n uri\n");
		printf("--------\n");

		printf("scheme = ");
		stringaa_print(&uri->scheme);
		printf("\n");

		printf("authority = ");
		stringaa_print(&uri->authority);
		printf("\n");

		printf("userinfo = ");
		stringaa_print(&uri->userinfo);
		printf("\n");

		printf("host = ");
		stringaa_print(&uri->host);
		printf("\n");

		printf("path = ");
		stringaa_print(&uri->path);
		printf("\n");

		printf("net_path = ");
		stringaa_print(&uri->net_path);
		printf("\n");

		printf("abs_path = ");
		stringaa_print(&uri->abs_path);
		printf("\n");

		printf("rel_path = ");
		stringaa_print(&uri->rel_path);
		printf("\n");

		printf("opaque_part = ");
		stringaa_print(&uri->opaque_part);
		printf("\n");

		printf("query = ");
		stringaa_print(&uri->query);
		printf("\n");

		printf("fragment = ");
		stringaa_print(&uri->fragment);
		printf("\n");

	return 1;
}

