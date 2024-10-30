/* HTTP/1.1 RFC 2616
 * Copyright (C) 2021  Askar Azhibaev
 *
 * File: httpaa_utils.h
 * Description: Implementation of HTTP/1.1 RFC 2616
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: December 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef HTTPAA_UTILS_H
#define HTTPAA_UTILS_H

/* Return 1 if parse message headers
 */
HTTPAA_INLINE int httpaa_parse_request_headers(httpaa *http, stringaa *request)
{
	int is_parse = 0;
	size_t pos = 0;

	pos = request->read_pos;

	if (http11_find_crlf(request))
	{
		http11_crlf(request);
		if (http11_find_crlf(request))
		{
			stringaa_begin_read_pos(request);
			/* Message headers complete */
			is_parse = http11_request(http, request);
		}
	}

	if (is_parse)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_parse;
}

HTTPAA_INLINE int httpaa_get_filename(uriaa *uri)
{
	int is_check = 0;

	printf("file: ");
	stringaa_fprint_line(stderr, &uri->abs_path);
	printf("\n");

	uriaa_remove_segment_point(&uri->abs_path);

	printf("file: ");
	stringaa_fprint_line(stderr, &uri->abs_path);
	printf("\n");

	uriaa_remove_segment_points(&uri->abs_path);

	printf("file: ");
	stringaa_fprint_line(stderr, &uri->abs_path);
	printf("\n");

	return is_check;
}

HTTPAA_INLINE int httpaa_build_response(httpaa *http, stringaa *request)
{
	/* Создание ответа сайта
	 * Проверить URL запроса
	 * uriaa: unescape CHARS_ESCAPE
	 * stringaa_http: - remove ./
	 * stringaa_http: - remove segment/../
	 * fileaa: check directory
	 * fileaa: check file (rights)
	 * fileaa: Если запрашивается файл, проверить файл на чтение
	 * archaa: Если запрашивается архив, открыть архив и прочитать содержание
	 * httpaa: Построить status_line 200 OK
	 * httpaa: Проверить connection
	 * httpaa: Передать имя сервера, дату ответа, режим кеширования
	 */
	int is_build = 0;

	httpaa_get_filename(&http->request.request_line.request_uri);

	return is_build;
}

HTTPAA_INLINE int httpaa_print(httpaa *http)
{
	int i = 0;
	int index = 0;
	const char *s_header = NULL;

	printf("Method: %s\n", http11_method_array[http->request.request_line.method]);
	printf("URL: ");
	stringaa_print(&http->request.request_line.request_uri.uri);
	printf("\n");
	printf("HTTP version: %d.%d\n", http->request.request_line.http_version.major_version, http->request.request_line.http_version.minor_version);
	for (i = HTTP11_GENERAL_HEADER_ARRAY_START; i < HTTP11_GENERAL_HEADER_ARRAY_SIZE + HTTP11_REQUEST_HEADER_ARRAY_SIZE + HTTP11_ENTITY_HEADER_ARRAY_SIZE; i++)
	{
		if (http->request.header[i].is_init)
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
			stringaa_print(&http->request.header[i]);
			printf("\n");
		}
	}

	return 0;
}

#endif	/* HTTPAA_UTILS_H */

