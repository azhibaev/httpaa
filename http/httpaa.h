/* HTTP/1.1 RFC 2616
 * Copyright (C) 2021  Askar Azhibaev
 *
 * File: httpaa.h
 * Description: Implementation of HTTP/1.1 RFC 2616
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: December 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef HTTPAA_H
#define HTTPAA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "uriaa.h"

#define HTTPAA_INLINE static inline

#define BUF_INT_SIZE 50

HTTPAA_INLINE int httpaa_print_time()
{
	time_t t = 0;
	time(&t);
	printf("%s|-", ctime(&t));
	return 0;
}

#ifdef _DEBUG
#define HTTPAA_DEBUG(p) {httpaa_print_time();p;}
#else
#define HTTPAA_DEBUG(p)
#endif

#include "httpaa_definitions.h"
#include "httpaa_arrays.h"
#include "httpaa_structs.h"
#include "httpaa_declarations.h"
#include "httpaa_utils.h"
#include "httpaa_work.h"

HTTPAA_INLINE int http11_crlf(stringaa *request)
{
	char c = 0;

	c = stringaa_get_char(request);

	if (HTTP11_CR)
	{
		stringaa_next_read_pos(request);
		c = stringaa_get_char(request);
		if (HTTP11_LF)
		{
			stringaa_next_read_pos(request);
			return 1;
		}
		else
		{
			stringaa_prev_read_pos(request);
		}
	}

	return 0;
}

HTTPAA_INLINE int http11_find_crlf(stringaa *request)
{
	char c = 0;
	size_t pos = 0;

	pos = request->read_pos;

	c = stringaa_get_char(request);

	while (request->read_pos < request->write_pos)
	{
		stringaa_next_read_pos(request);
		if (HTTP11_CR)
		{
			c = stringaa_get_char(request);
			if (HTTP11_LF)
			{
				stringaa_prev_read_pos(request);
				return 1;
			}
		}
		else
		{
			c = stringaa_get_char(request);
		}
	}

	request->read_pos = pos;

	return 0;
}

HTTPAA_INLINE int http11_lws(stringaa *request)
{
	char c = 0;
	int is_lws = 0;
	size_t pos = 0;

	pos = request->read_pos;

	if (http11_crlf(request))
	{
	}

	c = stringaa_get_char(request);
	if (HTTP11_SP || HTTP11_HT)
	{
		is_lws = 1;
		stringaa_next_read_pos(request);
		c = stringaa_get_char(request);
		while (HTTP11_SP || HTTP11_HT)
		{
			stringaa_next_read_pos(request);
			c = stringaa_get_char(request);
		}
	}

	if (is_lws)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_lws;
}

HTTPAA_INLINE int http11_text(stringaa *request)
{
	char c = 0;
	int is_text = 1;

	c = stringaa_get_char(request);

	if (HTTP11_CTL)
	{
		is_text = 0;
	}
	if (http11_lws(request))
	{
		is_text = 1;
	}

	return is_text;
}

HTTPAA_INLINE int http11_token(stringaa *request)
{
	char c = 0;
	int is_token = 0;

	c = stringaa_get_char(request);

	if (HTTP11_CHAR)
	{
		if (HTTP11_CTL || HTTP11_SEPARATORS)
		{
			return 0;
		}
		is_token = 1;
		stringaa_next_read_pos(request);
		c = stringaa_get_char(request);
		while (HTTP11_CHAR)
		{
			if (HTTP11_CTL || HTTP11_SEPARATORS)
			{
				break;
			}
			stringaa_next_read_pos(request);
			c = stringaa_get_char(request);
		}
	}

	return is_token;
}

HTTPAA_INLINE int http11_comment(stringaa *request)
{
	char c = 0;
	int is_comment = 0;
	size_t pos = 0;

	pos = request->read_pos;

	c = stringaa_get_char(request);

	if (c == '(')
	{
		stringaa_next_read_pos(request);
		while (http11_ctext(request) ||
				http11_quoted_pair(request) ||
				http11_comment(request))
		{
		}
		c = stringaa_get_char(request);
		if (c == ')')
		{
			is_comment = 1;
			stringaa_next_read_pos(request);
		}
	}

	if (is_comment)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_comment;
}

HTTPAA_INLINE int http11_ctext(stringaa *request)
{
	char c = 0;
	int is_ctext = 0;

	c = stringaa_get_char(request);

	if (c != '(' && c != ')')
	{
		if (http11_text(request))
		{
			is_ctext = 1;
		}
	}

	return is_ctext;
}

HTTPAA_INLINE int http11_quoted_string(stringaa *request)
{
	char c = 0;
	int is_quoted_string = 0;
	size_t pos = 0;

	pos = request->read_pos;

	c = stringaa_get_char(request);

	if (c == '"')
	{
		stringaa_next_read_pos(request);
		while (http11_qdtext(request) ||
				http11_quoted_pair(request))
		{
		}
		c = stringaa_get_char(request);
		if (c == '"')
		{
			is_quoted_string = 1;
			stringaa_next_read_pos(request);
		}
	}

	if (is_quoted_string)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_quoted_string;
}

HTTPAA_INLINE int http11_qdtext(stringaa *request)
{
	char c = 0;
	int is_qdtext = 0;

	c = stringaa_get_char(request);

	if (c != '"')
	{
		if (http11_text(request))
		{
			is_qdtext = 1;
		}
	}

	return is_qdtext;
}

HTTPAA_INLINE int http11_quoted_pair(stringaa *request)
{
	char c = 0;
	int is_quoted_pair = 0;

	c = stringaa_get_char(request);

	if (c == '\\')
	{
		stringaa_next_read_pos(request);
		c = stringaa_get_char(request);
		if (HTTP11_CHAR)
		{
			is_quoted_pair = 1;
			stringaa_next_read_pos(request);
		}
		else
		{
			stringaa_prev_read_pos(request);
		}
	}

	return is_quoted_pair;
}

HTTPAA_INLINE int http11_http_version(httpaa *http,
		stringaa *request)
{
	char c = 0;
	int is_http_version = 0;
	size_t pos = 0;
	size_t pos1 = 0;
	size_t pos2 = 0;
	size_t size1 = 0;
	size_t size2 = 0;
	char buf_int[BUF_INT_SIZE];
	const char *s_http = "HTTP/";
	const char *pc = NULL;

	pos = request->read_pos;

	c = stringaa_get_char(request);

	for (pc = s_http; *pc != 0; pc++)
	{
		if (*pc == _toupper(c))
		{
			stringaa_next_read_pos(request);
			c = stringaa_get_char(request);
		}
		else
		{
			break;
		}
	}
	if (*pc == 0)
	{
		is_http_version = 1;
	}
	
	if (is_http_version)
	{
		is_http_version = 0;
		if (HTTP11_DIGIT)
		{
			pos1 = request->read_pos;
			size1++;
			stringaa_next_read_pos(request);
			c = stringaa_get_char(request);
			while (HTTP11_DIGIT)
			{
				size1++;
				stringaa_next_read_pos(request);
				c = stringaa_get_char(request);
			}
			if (c == '.')
			{
				stringaa_next_read_pos(request);
				c = stringaa_get_char(request);
				if (HTTP11_DIGIT)
				{
					pos2 = request->read_pos;
					size2++;
					is_http_version = 1;
					stringaa_next_read_pos(request);
					c = stringaa_get_char(request);
					while (HTTP11_DIGIT)
					{
						size2++;
						stringaa_next_read_pos(request);
						c = stringaa_get_char(request);
					}
				}
			}
		}
	}

	if (is_http_version)
	{
		if (size1 >= BUF_INT_SIZE)
		{
			size1 = BUF_INT_SIZE - 1;
		}
		if (size2 >= BUF_INT_SIZE)
		{
			size2 = BUF_INT_SIZE - 1;
		}
		memcpy(buf_int, request->buf + pos1, size1);
		buf_int[size1] = 0;
		http->request.request_line.http_version.major_version = atoi(buf_int);
		memcpy(buf_int, request->buf + pos2, size2);
		buf_int[size2] = 0;
		http->request.request_line.http_version.minor_version = atoi(buf_int);
	}
	else
	{
		request->read_pos = pos;
	}

	return is_http_version;
}

HTTPAA_INLINE int http11_request(httpaa *http,
		stringaa *request)
{
	int is_request = 0;

	printf("request\n");
	if (http11_request_line(http, request))
	{
	printf("request line\n");
		is_request = 1;
		while (1)
		{
			if (http11_general_header(http, request) ||
					http11_request_header(http, request) ||
					http11_entity_header(http, request))
			{
				/*  found
				 *  test for CRLF 
				 */
				if (http11_crlf(request))
				{
					/*  message header completed
					 *  next cycle 
					 */
				}
				else
				{
					is_request = 0;
					break;
				}
			}
			/*  not found
			 *  if CRLF then message body 
			 */
			else if (http11_crlf(request))
			{
				if (http11_message_body(request))
				{
				}
				break;
			}
			/*  if not CRLF then unknown message header */
			else
			{
				/*  rewind to next CRLF */
				if (http11_find_crlf(request))
				{
					/*  test for CRLF */
					http11_crlf(request);
					/*  message header completed
					 *  next cycle 
					 */
				}
				/*  not CRLF
				 *  error 
				 */
				else
				{
					is_request = 0;
					break;
				}
			}
		}
	}

	return is_request;
}

HTTPAA_INLINE int http11_request_line(httpaa *http,
		stringaa *request)
{
	char c = 0;
	int is_request_line = 0;
	size_t pos = 0;

	pos = request->read_pos;

	if (http11_method(http, request))
	{	
		c = stringaa_get_char(request);
		if (HTTP11_SP)
		{
			stringaa_next_read_pos(request);
			if (http11_request_uri(http, request))
			{
				c = stringaa_get_char(request);
				if (HTTP11_SP)
				{
					stringaa_next_read_pos(request);
					if (http11_http_version(http, request))
					{
						if (http11_crlf(request))
						{
							is_request_line = 1;
						}
					}
				}
			}
		}
	}

	if (is_request_line)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_request_line;
}

HTTPAA_INLINE int http11_method(httpaa *http,
		stringaa *request)
{
	char c = 0;
	int is_method = 0;
	int i = 0;
	size_t pos = 0;
	const char *pc = NULL;

	pos = request->read_pos;

	c = stringaa_get_char(request);

	for (i = 0; i < HTTP11_METHOD_ARRAY_SIZE; i++)
	{
		for (pc = http11_method_array[i]; *pc != 0; pc++)
		{
			if (*pc == _toupper(c))
			{
				stringaa_next_read_pos(request);
				c = stringaa_get_char(request);
			}
			else
			{
				break;
			}
		}
		if (*pc == 0)
		{
			is_method = 1;
			break;
		}
	}

	if (is_method)
	{
		http->request.request_line.method = i;
	}
	else
	{
		request->read_pos = pos;
	}

	return is_method;
}

HTTPAA_INLINE int http11_general_header(httpaa *http,
		stringaa *request)
{
	char c = 0;
	int is_general_header = 0;
	int i = 0;
	size_t pos = 0;
	const char *pc = NULL;
	size_t buf_size = 0;

	pos = request->read_pos;

	c = _tolower(stringaa_get_char(request));

	for (i = 0; i < HTTP11_GENERAL_HEADER_ARRAY_SIZE; i++)
	{
		for (pc = http11_general_header_array[i]; *pc != 0; pc++)
		{
			if (*pc == c)
			{
				if (stringaa_next_read_pos(request))
				{
					c = _tolower(stringaa_get_char(request));
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		if (*pc == 0 && c == ':')
		{
			is_general_header = 1;
			break;
		}
		else if (request->read_pos != pos)
		{
			request->read_pos = pos;
			c = _tolower(stringaa_get_char(request));
		}
	}

	if (is_general_header)
	{
		pos = request->read_pos;
		if (http11_find_crlf(request))
		{
			buf_size = request->read_pos - pos;
		}
		else
		{
			buf_size = request->write_pos - pos;
		}
		stringaa_set_chars_size(&http->request.header[HTTP11_GENERAL_HEADER_ARRAY_START + i], request->buf + pos, buf_size);
	}
	else
	{
		request->read_pos = pos;
	}

	return is_general_header;
}

HTTPAA_INLINE int http11_request_header(httpaa *http,
		stringaa *request)
{
	char c = 0;
	int is_request_header = 0;
	int i = 0;
	size_t pos = 0;
	const char *pc = NULL;
	size_t buf_size = 0;

	pos = request->read_pos;

	c = _tolower(stringaa_get_char(request));

	for (i = 0; i < HTTP11_REQUEST_HEADER_ARRAY_SIZE; i++)
	{
		for (pc = http11_request_header_array[i]; *pc != 0; pc++)
		{
			if (*pc == c)
			{
				if (stringaa_next_read_pos(request))
				{
					c = _tolower(stringaa_get_char(request));
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		if (*pc == 0 && c == ':')
		{
			is_request_header = 1;
			break;
		}
		else if (request->read_pos != pos)
		{
			request->read_pos = pos;
			c = _tolower(stringaa_get_char(request));
		}
	}

	if (is_request_header)
	{
		pos = request->read_pos;
		if (http11_find_crlf(request))
		{
			buf_size = request->read_pos - pos;
		}
		else
		{
			buf_size = request->write_pos - pos;
		}
		stringaa_set_chars_size(&http->request.header[HTTP11_REQUEST_HEADER_ARRAY_START + i], request->buf + pos, buf_size);
	}
	else
	{
		request->read_pos = pos;
	}

	return is_request_header;
}

HTTPAA_INLINE int http11_response_header(httpaa *http,
		stringaa *request)
{
	char c = 0;
	int is_response_header = 0;
	int i = 0;
	size_t pos = 0;
	const char *pc = NULL;
	size_t buf_size = 0;

	pos = request->read_pos;

	c = _tolower(stringaa_get_char(request));

	for (i = 0; i < HTTP11_RESPONSE_HEADER_ARRAY_SIZE; i++)
	{
		for (pc = http11_response_header_array[i]; *pc != 0; pc++)
		{
			if (*pc == c)
			{
				if (stringaa_next_read_pos(request))
				{
					c = _tolower(stringaa_get_char(request));
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		if (*pc == 0 && c == ':')
		{
			is_response_header = 1;
			break;
		}
		else if (request->read_pos != pos)
		{
			request->read_pos = pos;
			c = _tolower(stringaa_get_char(request));
		}
	}

	if (is_response_header)
	{
		pos = request->read_pos;
		if (http11_find_crlf(request))
		{
			buf_size = request->read_pos - pos;
		}
		else
		{
			buf_size = request->write_pos - pos;
		}
		stringaa_set_chars_size(&http->request.header[HTTP11_RESPONSE_HEADER_ARRAY_START + i], request->buf + pos, buf_size);
	}
	else
	{
		request->read_pos = pos;
	}

	return is_response_header;
}

HTTPAA_INLINE int http11_entity_header(httpaa *http,
		stringaa *request)
{
	char c = 0;
	int is_entity_header = 0;
	int i = 0;
	size_t pos = 0;
	const char *pc = NULL;
	size_t buf_size = 0;

	pos = request->read_pos;

	c = _tolower(stringaa_get_char(request));

	for (i = 0; i < HTTP11_ENTITY_HEADER_ARRAY_SIZE; i++)
	{
		for (pc = http11_entity_header_array[i]; *pc != 0; pc++)
		{
			if (*pc == c)
			{
				if (stringaa_next_read_pos(request))
				{
					c = _tolower(stringaa_get_char(request));
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		if (*pc == 0 && c == ':')
		{
			is_entity_header = 1;
			break;
		}
		else if (request->read_pos != pos)
		{
			request->read_pos = pos;
			c = _tolower(stringaa_get_char(request));
		}
	}

	if (is_entity_header)
	{
		pos = request->read_pos;
		if (http11_find_crlf(request))
		{
			buf_size = request->read_pos - pos;
		}
		else
		{
			buf_size = request->write_pos - pos;
		}
		stringaa_set_chars_size(&http->request.header[HTTP11_ENTITY_HEADER_ARRAY_START + i], request->buf + pos, buf_size);
	}
	else
	{
		request->read_pos = pos;
	}

	return is_entity_header;
}

HTTPAA_INLINE int http11_message_body(stringaa *request)
{
	return 0;
}

HTTPAA_INLINE int http11_request_uri(httpaa *http,
		stringaa *request)
{
	int is_request_uri = 0;

	stringaa_set_substring(&http->request.request_line.request_uri.uri, request, STRINGAA_READ_POS, STRINGAA_WRITE_POS);
	if (uriaa_check_url(&http->request.request_line.request_uri))
	{
		is_request_uri = 1;
		http->request.request_line.request_uri.uri.write_pos = http->request.request_line.request_uri.uri.read_pos;
		request->read_pos += http->request.request_line.request_uri.uri.read_pos;
	}

	return is_request_uri;
}

HTTPAA_INLINE int http11_message_header(httpaa *http,
		stringaa *request)
{
	int is_message_header = 0;
	size_t pos = 0;

	pos = request->buf[request->read_pos];

	if (http11_field_name(http, request))
	{
		if (stringaa_is_char(request, ':'))
		{
			if (http11_field_value(http, request))
			{
			}
		}
	}

	if (is_message_header)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_message_header;
}

HTTPAA_INLINE int http11_field_name(httpaa *http,
        stringaa *request)
{
	int is_field_name = 0;
	size_t pos = 0;

	pos = request->read_pos;

	if (http11_token(request))
	{
		is_field_name = 1;
	}

	if (is_field_name)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_field_name;
}

HTTPAA_INLINE int http11_field_value(httpaa *http,
        stringaa *request)
{
	int is_field_value = 0;
	size_t pos = 0;

	pos = request->read_pos;

	while (1)
	{
		if (http11_lws(request))
		{
			is_field_value = 1;
		}
		else if (http11_field_content(http, request))
		{
			is_field_value = 1;
		}
		else
		{
			break;
		}
	}

	if (is_field_value)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_field_value;
}

HTTPAA_INLINE int http11_field_content(httpaa *http,
        stringaa *request)
{
	char c = 0;
	int is_field_content = 0;
	size_t pos = 0;

	pos = request->read_pos;

	while (1)
	{
		if (http11_text(request))
		{
		}
		else if (http11_token(request))
		{
		}
		else if (http11_quoted_string(request))
		{
		}
		else
		{
			c = stringaa_get_char(request);
			if (HTTP11_SEPARATORS)
			{
			}
		}
	}

	return is_field_content;
}

HTTPAA_INLINE int http11_header_cache_control(httpaa *http,
        stringaa *request)
{
	int is_cache_control = 0;

	while (1)
	{
		if (http11_header_cache_directive(http, request))
		{
			is_cache_control = 1;
		}

		if (http11_lws(request))
		{
		}

		if (stringaa_is_char(request, ','))
		{
		}

		if (http11_lws(request))
		{
		}
	}

	return is_cache_control;
}

HTTPAA_INLINE int http11_header_cache_directive(httpaa *http,
        stringaa *request)
{
	int is_cache_directive = 0;

	if (http11_header_cache_request_directive(http, request) ||
			http11_header_cache_response_directive(http, request))
	{
		is_cache_directive = 1;
	}

	return is_cache_directive;
}

HTTPAA_INLINE int http11_header_cache_request_directive(httpaa *http,
        stringaa *request)
{
	char c = 0;
	int is_cache_request_directive = 0;
	int i = 0;
	size_t pos = 0;
	const char *pc = NULL;

	pos = request->read_pos;

	c = stringaa_get_char(request);

	for (i = 0; i < HTTP11_HEADER_CACHE_REQUEST_DIRECTIVE_ARRAY_SIZE; i++)
	{
		for (pc = http11_header_cache_request_directive_array[i]; *pc != 0; pc++)
		{
			if (*pc == _tolower(c))
			{
				stringaa_next_read_pos(request);
				c = stringaa_get_char(request);
			}
			else
			{
				break;
			}
		}
		if (*pc == 0)
		{
			is_cache_request_directive = 1;
			break;
		}
	}

	if (is_cache_request_directive)
	{
		switch (i)
		{
			case http11_header_cache_request_directive_max_age:
				if (stringaa_is_char(request, '='))
				{
					if (http11_delta_seconds(request))
					{
					}
				}
				break;
			case http11_header_cache_request_directive_max_stale:
				if (stringaa_is_char(request, '='))
				{
					if (http11_delta_seconds(request))
					{
					}
				}
				break;
			case http11_header_cache_request_directive_min_fresh:
				if (stringaa_is_char(request, '='))
				{
					if (http11_delta_seconds(request))
					{
					}
				}
				break;
		};
	}
	else
	{
		request->read_pos = pos;
	}

	return is_cache_request_directive;
}

HTTPAA_INLINE int http11_header_cache_response_directive(httpaa *http,
        stringaa *request)
{
	char c = 0;
	int is_cache_response_directive = 0;
	int i = 0;
	size_t pos = 0;
	const char *pc = NULL;

	pos = request->read_pos;

	c = stringaa_get_char(request);

	for (i = 0; i < HTTP11_HEADER_CACHE_RESPONSE_DIRECTIVE_ARRAY_SIZE; i++)
	{
		for (pc = http11_header_cache_response_directive_array[i]; *pc != 0; pc++)
		{
			if (*pc == _tolower(c))
			{
				stringaa_next_read_pos(request);
				c = stringaa_get_char(request);
			}
			else
			{
				break;
			}
		}
		if (*pc == 0)
		{
			is_cache_response_directive = 1;
			break;
		}
	}

	if (is_cache_response_directive)
	{
		switch (i)
		{
			case http11_header_cache_response_directive_private:
				if (stringaa_is_char(request, '='))
				{
					if (stringaa_is_char(request, '"'))
					{
						while (1)
						{
							if (http11_field_name(http, request))
							{
							}
							if (http11_lws(request))
							{
							}
							if (stringaa_is_char(request, ','))
							{
							}
							if (http11_lws(request))
							{
							}
						}
						if (stringaa_is_char(request, '"'))
						{
						}
					}
				}
				break;
			case http11_header_cache_response_directive_no_cache:
				if (stringaa_is_char(request, '='))
				{
					if (stringaa_is_char(request, '"'))
					{
						while (1)
						{
							if (http11_field_name(http, request))
							{
							}
							if (http11_lws(request))
							{
							}
							if (stringaa_is_char(request, ','))
							{
							}
							if (http11_lws(request))
							{
							}
						}
						if (stringaa_is_char(request, '"'))
						{
						}
					}
				}
				break;
			case http11_header_cache_response_directive_max_age:
				if (stringaa_is_char(request, '='))
				{
					if (http11_delta_seconds(request))
					{
					}
				}
				break;
			case http11_header_cache_response_directive_s_maxage:
				if (stringaa_is_char(request, '='))
				{
					if (http11_delta_seconds(request))
					{
					}
				}
				break;
		};
	}
	else
	{
		request->read_pos = pos;
	}

	return is_cache_response_directive;
}

HTTPAA_INLINE int http11_header_connection(httpaa *http,
        stringaa *request)
{
	int is_header_connection = 0;

	while (1)
	{
		if (http11_token(request))
		{
		}
		if (http11_lws(request))
		{
		}
		if (stringaa_is_char(request, ','))
		{
		}
		if (http11_lws(request))
		{
		}
	}

	return is_header_connection;
}

HTTPAA_INLINE int http11_header_date(httpaa *http,
        stringaa *request)
{
	int is_header_date = 0;

	if (http11_date(request))
	{
		is_header_date = 1;
	}

	return is_header_date;
}

HTTPAA_INLINE int http11_date(stringaa *request)
{
	int is_date = 0;

	if (http11_rfc1123_date(request))
	{
		is_date = 1;
	}
	else if (http11_rfc850_date(request))
	{
		is_date = 1;
	}
	else if (http11_asctime_date(request))
	{
		is_date = 1;
	}

	return is_date;
}

HTTPAA_INLINE int http11_rfc1123_date(stringaa *request)
{
	char c = 0;
	int is_rfc1123_date = 0;
	size_t pos = 0;
	const char *s_gmt = "GMT";
	const char *pc = NULL;
	int is_gmt = 0;

	pos = request->read_pos;

	if (http11_wkday(request))
	{
		if (stringaa_is_char(request, ','))
		{
			c = stringaa_get_char(request);
			if (HTTP11_SP)
			{
				stringaa_next_read_pos(request);
				if (http11_date1(request))
				{
					c = stringaa_get_char(request);
					if (HTTP11_SP)
					{
						stringaa_next_read_pos(request);
						if (http11_time(request))
						{
							c = stringaa_get_char(request);
							if (HTTP11_SP)
							{
								stringaa_next_read_pos(request);
								c = stringaa_get_char(request);
								for (pc = s_gmt; *pc != 0; pc++)
								{
									if (*pc == _toupper(c))
									{
										stringaa_next_read_pos(request);
										c = stringaa_get_char(request);
									}
									else
									{
										break;
									}
								}
								if (*pc == 0)
								{
									is_gmt = 1;
								}
							}
						}
					}
				}
			}
		}
	}

	is_rfc1123_date = is_gmt;

	if (is_rfc1123_date)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_rfc1123_date;
}

HTTPAA_INLINE int http11_rfc850_date(stringaa *request)
{
	char c = 0;
	int is_rfc850_date = 0;
	size_t pos = 0;
	const char *s_gmt = "GMT";
	const char *pc = NULL;
	int is_gmt = 0;

	pos = request->read_pos;

	if (http11_weekday(request))
	{
		if (stringaa_is_char(request, ','))
		{
			c = stringaa_get_char(request);
			if (HTTP11_SP)
			{
				stringaa_next_read_pos(request);
				if (http11_date2(request))
				{
					c = stringaa_get_char(request);
					if (HTTP11_SP)
					{
						stringaa_next_read_pos(request);
						if (http11_time(request))
						{
							c = stringaa_get_char(request);
							if (HTTP11_SP)
							{
								stringaa_next_read_pos(request);
								c = stringaa_get_char(request);
								for (pc = s_gmt; *pc != 0; pc++)
								{
									if (*pc == _toupper(c))
									{
										stringaa_next_read_pos(request);
										c = stringaa_get_char(request);
									}
									else
									{
										break;
									}
								}
								if (*pc == 0)
								{
									is_gmt = 1;
								}
							}
						}
					}
				}
			}
		}
	}

	is_rfc850_date = is_gmt;

	if (is_rfc850_date)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_rfc850_date;
}

HTTPAA_INLINE int http11_asctime_date(stringaa *request)
{
	char c = 0;
	int is_asctime_date = 0;
	size_t pos = 0;
	int i = 0;

	pos = request->read_pos;

	if (http11_wkday(request))
	{
		c = stringaa_get_char(request);
		if (HTTP11_SP)
		{
			stringaa_next_read_pos(request);
			if (http11_date3(request))
			{
				c = stringaa_get_char(request);
				if (HTTP11_SP)
				{
					stringaa_next_read_pos(request);
					if (http11_time(request))
					{
						c = stringaa_get_char(request);
						if (HTTP11_SP)
						{
							stringaa_next_read_pos(request);
							c = stringaa_get_char(request);
							for (i = 0; i < 4; i++)
							{
								if (HTTP11_DIGIT)
								{
									stringaa_next_read_pos(request);
									c = stringaa_get_char(request);
								}
								else
								{
									break;
								}
							}
							if (i == 4)
							{
								is_asctime_date = 1;
							}
						}
					}
				}
			}
		}
	}

	if (is_asctime_date)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_asctime_date;
}

HTTPAA_INLINE int http11_date1(stringaa *request)
{
	char c = 0;
	int is_date1 = 0;
	size_t pos = 0;
	int i = 0;

	pos = request->read_pos;

	c = stringaa_get_char(request);
	for (i = 0; i < 2; i++)
	{
		if (HTTP11_DIGIT)
		{
			stringaa_next_read_pos(request);
			c = stringaa_get_char(request);
		}
		else
		{
			break;
		}
	}
	if (i == 2)
	{
		if (HTTP11_SP)
		{
			stringaa_next_read_pos(request);
			if (http11_month(request))
			{
				c = stringaa_get_char(request);
				if (HTTP11_SP)
				{
					stringaa_next_read_pos(request);
					c = stringaa_get_char(request);
					for (i = 0; i < 4; i++)
					{
						if (HTTP11_DIGIT)
						{
							stringaa_next_read_pos(request);
							c = stringaa_get_char(request);
						}
						else
						{
							break;
						}
					}
					if (i == 4)
					{
						is_date1 = 1;
					}
				}
			}
		}
	}

	if (is_date1)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_date1;
}

HTTPAA_INLINE int http11_date2(stringaa *request)
{
	char c = 0;
	int is_date2 = 0;
	size_t pos = 0;
	int i = 0;

	pos = request->read_pos;

	c = stringaa_get_char(request);
	for (i = 0; i < 2; i++)
	{
		if (HTTP11_DIGIT)
		{
			stringaa_next_read_pos(request);
			c = stringaa_get_char(request);
		}
		else
		{
			break;
		}
	}
	if (i == 2)
	{
		if (c == '-')
		{
			stringaa_next_read_pos(request);
			if (http11_month(request))
			{
				c = stringaa_get_char(request);
				if (c == '-')
				{
					stringaa_next_read_pos(request);
					c = stringaa_get_char(request);
					for (i = 0; i < 2; i++)
					{
						if (HTTP11_DIGIT)
						{
							stringaa_next_read_pos(request);
							c = stringaa_get_char(request);
						}
						else
						{
							break;
						}
					}
					if (i == 2)
					{
						is_date2 = 1;
					}
				}
			}
		}
	}

	if (is_date2)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_date2;
}

HTTPAA_INLINE int http11_date3(stringaa *request)
{
	char c = 0;
	int is_date3 = 0;
	size_t pos = 0;
	int i = 0;
	size_t digit_pos = 0;

	pos = request->read_pos;

	if (http11_month(request))
	{
		c = stringaa_get_char(request);
		if (HTTP11_SP)
		{
			stringaa_next_read_pos(request);
			c = stringaa_get_char(request);
			digit_pos = request->read_pos;
			for (i = 0; i < 2; i++)
			{
				if (HTTP11_DIGIT)
				{
					stringaa_next_read_pos(request);
					c = stringaa_get_char(request);
				}
				else
				{
					break;
				}
			}
			if (i != 2)
			{
				request->read_pos = digit_pos;
				c = stringaa_get_char(request);
				if (HTTP11_SP)
				{
					stringaa_next_read_pos(request);
					c = stringaa_get_char(request);
					if (HTTP11_DIGIT)
					{
						stringaa_next_read_pos(request);
						is_date3 = 1;
					}
				}
			}
		}
	}

	if (is_date3)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_date3;
}

HTTPAA_INLINE int http11_time(stringaa *request)
{
	char c = 0;
	int is_time = 0;
	size_t pos = 0;
	int i = 0;

	pos = request->read_pos;

	c = stringaa_get_char(request);
	for (i = 0; i < 2; i++)
	{
		if (HTTP11_DIGIT)
		{
			stringaa_next_read_pos(request);
			c = stringaa_get_char(request);
		}
		else
		{
			break;
		}
	}
	if (i == 2)
	{
		if (c == ':')
		{
			stringaa_next_read_pos(request);
			c = stringaa_get_char(request);
			for (i = 0; i < 2; i++)
			{
				if (HTTP11_DIGIT)
				{
					stringaa_next_read_pos(request);
					c = stringaa_get_char(request);
				}
				else
				{
					break;
				}
			}
			if (i == 2)
			{	
				if (c == ':')
				{
					stringaa_next_read_pos(request);
					c = stringaa_get_char(request);
					for (i = 0; i < 2; i++)
					{
						if (HTTP11_DIGIT)
						{
							stringaa_next_read_pos(request);
							c = stringaa_get_char(request);
						}
						else
						{
							break;
						}
					}
					if (i == 2)
					{
						is_time = 1;
					}
				}
			}
		}
	}

	if (is_time)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_time;
}

HTTPAA_INLINE int http11_wkday(stringaa *request)
{
	char c = 0;
	int is_wkday = 0;
	int i = 0;
	size_t pos = 0;
	const char *pc = NULL;

	pos = request->read_pos;

	c = stringaa_get_char(request);

	for (i = 0; i < HTTP11_DATE_WKDAY_ARRAY_SIZE; i++)
	{
		for (pc = http11_date_wkday_array[i]; *pc != 0; pc++)
		{
			if (_tolower(*pc) == _tolower(c))
			{
				stringaa_next_read_pos(request);
				c = stringaa_get_char(request);
			}
			else
			{
				break;
			}
		}
		if (*pc == 0)
		{
			is_wkday = 1;
			break;
		}
	}

	return is_wkday;
}

HTTPAA_INLINE int http11_weekday(stringaa *request)
{
	char c = 0;
	int is_weekday = 0;
	int i = 0;
	size_t pos = 0;
	const char *pc = NULL;

	pos = request->read_pos;

	c = stringaa_get_char(request);

	for (i = 0; i < HTTP11_DATE_WEEKDAY_ARRAY_SIZE; i++)
	{
		for (pc = http11_date_weekday_array[i]; *pc != 0; pc++)
		{
			if (_tolower(*pc) == _tolower(c))
			{
				stringaa_next_read_pos(request);
				c = stringaa_get_char(request);
			}
			else
			{
				break;
			}
		}
		if (*pc == 0)
		{
			is_weekday = 1;
			break;
		}
	}

	return is_weekday;
}

HTTPAA_INLINE int http11_month(stringaa *request)
{
	char c = 0;
	int is_month = 0;
	int i = 0;
	size_t pos = 0;
	const char *pc = NULL;

	pos = request->read_pos;

	c = stringaa_get_char(request);

	for (i = 0; i < HTTP11_DATE_MONTH_ARRAY_SIZE; i++)
	{
		for (pc = http11_date_month_array[i]; *pc != 0; pc++)
		{
			if (_tolower(*pc) == _tolower(c))
			{
				stringaa_next_read_pos(request);
				c = stringaa_get_char(request);
			}
			else
			{
				break;
			}
		}
		if (*pc == 0)
		{
			is_month = 1;
			break;
		}
	}

	return is_month;
}

HTTPAA_INLINE int http11_delta_seconds(stringaa *request)
{
	char c = 0;
	int is_delta_seconds = 0;
	size_t pos = 0;

	pos = request->read_pos;

	c = stringaa_get_char(request);

	while (1)
	{
		if (HTTP11_DIGIT)
		{
			stringaa_next_read_pos(request);
			c = stringaa_get_char(request);
		}
		else
		{
			break;
		}
	}

	return is_delta_seconds;
}

HTTPAA_INLINE int http11_header_pragma(httpaa *http,
        stringaa *request)
{
	int is_pragma = 0;

	while (1)
	{
		if (http11_header_pragma_directive(request))
		{
		}
		if (http11_lws(request))
		{
		}
		if (stringaa_is_char(request, ','))
		{
		}
		if (http11_lws(request))
		{
		}
	}

	return is_pragma;
}

HTTPAA_INLINE int http11_header_pragma_directive(stringaa *request)
{
	char c = 0;
	int is_pragma_directive = 0;
	int i = 0;
	size_t pos = 0;
	const char *pc = NULL;

	pos = request->read_pos;

	c = stringaa_get_char(request);

	for (i = 0; i < HTTP11_HEADER_PRAGMA_DIRECTIVE_ARRAY_SIZE; i++)
	{
		for (pc = http11_header_pragma_directive_array[i]; *pc != 0; pc++)
		{
			if (*pc == _tolower(c))
			{
				stringaa_next_read_pos(request);
				c = stringaa_get_char(request);
			}
			else
			{
				break;
			}
		}
		if (*pc == 0)
		{
			is_pragma_directive = 1;
			break;
		}
	}

	if (is_pragma_directive)
	{
	}
	else
	{
		if (http11_header_extension_pragma(request))
		{
		}
		else
		{
			request->read_pos = pos;
		}
	}

	return is_pragma_directive;
}

HTTPAA_INLINE int http11_header_extension_pragma(stringaa *request)
{
	int is_extension_pragma = 0;

	if (http11_token(request))
	{
		if (stringaa_is_char(request, '='))
		{
			if (http11_token(request))
			{
			}
			if (http11_quoted_string(request))
			{
			}
		}
	}

	return is_extension_pragma;
}

HTTPAA_INLINE int http11_header_trailer(httpaa *http,
        stringaa *request)
{
	int is_trailer = 0;

	while (1)
	{
		if (http11_field_name(http, request))
		{
		}
		if (http11_lws(request))
		{
		}
		if (stringaa_is_char(request, ','))
		{
		}
		if (http11_lws(request))
		{
		}
	}

	return is_trailer;
}

HTTPAA_INLINE int http11_header_transfer_encoding(httpaa *http,
        stringaa *request)
{
	int is_transfer_encoding = 0;

	while (1)
	{
		if (http11_header_transfer_coding(http, request))
		{
		}
		if (http11_lws(request))
		{
		}
		if (stringaa_is_char(request, ','))
		{
		}
		if (http11_lws(request))
		{
		}
	}

	return is_transfer_encoding;
}

HTTPAA_INLINE int http11_header_transfer_coding(httpaa *http,
        stringaa *request)
{
	char c = 0;
	int is_transfer_coding = 0;
	int i = 0;
	size_t pos = 0;
	const char *pc = NULL;

	pos = request->read_pos;

	c = stringaa_get_char(request);

	for (i = 0; i < HTTP11_HEADER_TRANSFER_CODING_ARRAY_SIZE; i++)
	{
		for (pc = http11_header_transfer_coding_array[i]; *pc != 0; pc++)
		{
			if (*pc == _tolower(c))
			{
				stringaa_next_read_pos(request);
				c = stringaa_get_char(request);
			}
			else
			{
				break;
			}
		}
		if (*pc == 0)
		{
			is_transfer_coding = 1;
			break;
		}
	}

	if (is_transfer_coding)
	{
	}
	else
	{
		if (http11_header_transfer_extension(http, request))
		{
		}
		else
		{
		request->read_pos = pos;
		}
	}

	return is_transfer_coding;
}

HTTPAA_INLINE int http11_header_transfer_extension(httpaa *http,
        stringaa *request)
{
	int is_transfer_extension = 0;

	if (http11_token(request))
	{
		if (stringaa_is_char(request, ';'))
		{
			if (http11_parameter(request))
			{
			}
		}
	}

	return is_transfer_extension;
}

HTTPAA_INLINE int http11_parameter(stringaa *request)
{
	int is_parameter = 0;

	/* attribute */
	if (http11_token(request))
	{
		if (stringaa_is_char(request, '='))
		{
			/* value */
			if (http11_token(request))
			{
			}
			else if (http11_quoted_string(request))
			{
			}
		}
	}

	return is_parameter;
}

HTTPAA_INLINE int http11_header_upgrade(httpaa *http,
        stringaa *request)
{
	int is_upgrade = 0;

	while (1)
	{
		if (http11_product(request))
		{
		}
		if (http11_lws(request))
		{
		}
		if (stringaa_is_char(request, ','))
		{
		}
		if (http11_lws(request))
		{
		}
	}

	return is_upgrade;
}

HTTPAA_INLINE int http11_product(stringaa *request)
{
	int is_product = 0;
	size_t pos = 0;

	pos = request->read_pos;

	if (http11_token(request))
	{
		is_product = 1;
		if (stringaa_is_char(request, '/'))
		{
			if (http11_token(request))
			{
			}
		}
	}

	if (is_product)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_product;
}

HTTPAA_INLINE int http11_header_via(httpaa *http,
        stringaa *request)
{
	int is_via = 0;

	while (1)
	{
		if (http11_received_protocol(request))
		{
		}
		if (http11_received_by(request))
		{
		}
		if (http11_comment(request))
		{
		}
	}

	return is_via;
}

HTTPAA_INLINE int http11_received_protocol(stringaa *request)
{
	int is_received_protocol = 0;

	/* protocol-name */
	if (http11_token(request))
	{
		if (stringaa_is_char(request, '/'))
		{
		}
	}
	/* protocol-version */
	if (http11_token(request))
	{
	}

	return is_received_protocol;
}

HTTPAA_INLINE int http11_received_by(stringaa *request)
{
	int is_received_by = 0;

	if (http11_host(request))
	{
		if (stringaa_is_char(request, ':'))
		{
			if (http11_port(request))
			{
			}
		}
	}
	/* pseudonym */
	else if (http11_token(request))
	{
	}

	return is_received_by;
}

HTTPAA_INLINE int http11_host(stringaa *request)
{
	int is_host = 0;
	struct uri_struct uri = {{0}};

	if (host(&uri))
	{
	}

	return is_host;
}

HTTPAA_INLINE int http11_port(stringaa *request)
{
	int is_port = 0;
	struct uri_struct uri = {{0}};

	if (port(&uri))
	{
	}

	return is_port;
}

HTTPAA_INLINE int http11_header_warning(httpaa *http,
        stringaa *request)
{
	int is_warning = 0;

	while (1)
	{
		if (http11_warning_value(request))
		{
		}
		if (http11_lws(request))
		{
		}
		if (stringaa_is_char(request, ','))
		{
		}
		if (http11_lws(request))
		{
		}
	}

	return is_warning;
}

HTTPAA_INLINE int http11_warning_value(stringaa *request)
{
	char c = 0;
	int is_warning_value = 0;
	size_t pos = 0;

	pos = request->read_pos;

	if (http11_warning_code(request))
	{
		c = stringaa_get_char(request);
		if (HTTP11_SP)
		{
			stringaa_next_read_pos(request);
			if (http11_warning_agent(request))
			{
				c = stringaa_get_char(request);
				if (HTTP11_SP)
				{
					stringaa_next_read_pos(request);
					/* warn-text */
					if (http11_quoted_string(request))
					{
						c = stringaa_get_char(request);
						if (HTTP11_SP)
						{
							stringaa_next_read_pos(request);
							/* warn-date */
							if (stringaa_is_char(request, '"'))
							{
								if (http11_date(request))
								{
									if (stringaa_is_char(request, '"'))
									{
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return is_warning_value;
}

HTTPAA_INLINE int http11_warning_code(stringaa *request)
{
	char c = 0;
	int is_warning_code = 0;
	size_t pos = 0;

	pos = request->read_pos;
	c = stringaa_get_char(request);

	if (HTTP11_DIGIT)
	{
		stringaa_next_read_pos(request);
		c = stringaa_get_char(request);
		if (HTTP11_DIGIT)
		{
			stringaa_next_read_pos(request);
			c = stringaa_get_char(request);
			if (HTTP11_DIGIT)
			{
				stringaa_next_read_pos(request);
				is_warning_code = 1;
			}
		}
	}

	if (is_warning_code)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_warning_code;
}

HTTPAA_INLINE int http11_warning_agent(stringaa *request)
{
	int is_warning_agent = 0;

	if (http11_host(request))
	{
		if (stringaa_is_char(request, ':'))
		{
			if (http11_port(request))
			{
			}
		}
	}
	else if (http11_token(request))
	{
	}

	return is_warning_agent;
}

HTTPAA_INLINE int http11_header_accept(httpaa *http,
        stringaa *request)
{
	int is_accept = 0;

	while (1)
	{
		if (http11_media_range(request))
		{
			if (http11_accept_params(request))
			{
			}
		}
	}

	return is_accept;
}

HTTPAA_INLINE int http11_media_range(stringaa *request)
{
	int is_media_range = 0;
	size_t pos = 0;

	pos = request->read_pos;

	if (stringaa_is_char(request, '*'))
	{
		if (stringaa_is_char(request, '/'))
		{
			if (stringaa_is_char(request, '*'))
			{
				is_media_range = 1;
			}
		}
	}

	if (!is_media_range)
	{
		if (http11_token(request))
		{
			if (stringaa_is_char(request, '/'))
			{
				if (stringaa_is_char(request, '*'))
				{
					is_media_range = 1;
				}
			}
		}
	}

	if (!is_media_range)
	{
		if (http11_token(request))
		{
			if (stringaa_is_char(request, '/'))
			{
				if (http11_token(request))
				{
					is_media_range = 1;
				}
			}
		}
	}

	if (is_media_range)
	{
		while (1)
		{
			if (stringaa_is_char(request, ';'))
			{
				if (http11_parameter(request))
				{
				}
			}
		}
	}

	if (is_media_range)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_media_range;
}

HTTPAA_INLINE int http11_accept_params(stringaa *request)
{
	int is_accept_params = 0;

	if (stringaa_is_char(request, ';'))
	{
		if (stringaa_is_chari(request, 'q'))
		{
			if (stringaa_is_char(request, '='))
			{
				if (http11_qvalue(request))
				{
					while (1)
					{
						if (http11_accept_extension(request))
						{
						}
					}
				}
			}
		}
	}

	return is_accept_params;
}

HTTPAA_INLINE int http11_accept_extension(stringaa *request)
{
	int is_accept_extension = 0;

	if (stringaa_is_char(request, ';'))
	{
		if (http11_token(request))
		{
			if (stringaa_is_char(request, '='))
			{
				if (http11_token(request))
				{
				}
				else if (http11_quoted_string(request))
				{
				}
			}
		}
	}

	return is_accept_extension;
}

HTTPAA_INLINE int http11_qvalue(stringaa *request)
{
	char c = 0;
	int is_qvalue = 0;
	size_t pos = 0;
	int i = 0;

	pos = request->read_pos;

	if (stringaa_is_char(request, '0'))
	{
		is_qvalue = 1;
		if (stringaa_is_char(request, '.'))
		{
			c = stringaa_get_char(request);
			for (i = 0; i < 3; i++)
			{
				if (HTTP11_DIGIT)
				{
					stringaa_next_read_pos(request);
					c = stringaa_get_char(request);
				}
				else
				{
					break;
				}
			}
		}
	}
	else if (stringaa_is_char(request, '1'))
		{
			is_qvalue = 1;
			if (stringaa_is_char(request, '.'))
			{
				c = stringaa_get_char(request);
				for (i = 0; i < 3; i++)
				{
					if (c == '0')
					{
						stringaa_next_read_pos(request);
						c = stringaa_get_char(request);
					}
					else
					{
						break;
					}
				}
			}
		}

	if (is_qvalue)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_qvalue;
}

HTTPAA_INLINE int http11_header_accept_charset(httpaa *http,
        stringaa *request)
{
	int is_accept_charset = 0;

	while (1)
	{
		if (http11_charset(request))
		{
		}
		else if (stringaa_is_char(request, '*'))
		{
		}
		if (stringaa_is_char(request, ';'))
		{
			if (stringaa_is_chari(request, 'q'))
			{
				if (stringaa_is_char(request, '='))
				{
					if (http11_qvalue(request))
					{
					}
				}
			}
		}
	}

	return is_accept_charset;
}

HTTPAA_INLINE int http11_charset(stringaa *request)
{
	return 0;
}

HTTPAA_INLINE int http11_header_accept_encoding(httpaa *http,
        stringaa *request)
{
	int is_accept_encoding = 0;

	while (1)
	{
		if (http11_codings(request))
		{
			if (stringaa_is_char(request, ';'))
			{
				if (stringaa_is_chari(request, 'q'))
				{
					if (stringaa_is_char(request, '='))
					{
						if (http11_qvalue(request))
						{
						}
					}
				}
			}
		}
	}

	return is_accept_encoding;
}

HTTPAA_INLINE int http11_codings(stringaa *request)
{
	int is_codings = 0;

	if (http11_content_coding(request))
	{
	}
	else if (stringaa_is_char(request, '*'))
	{
	}

	return is_codings;
}

HTTPAA_INLINE int http11_content_coding(stringaa *request)
{
	return 0;
}

HTTPAA_INLINE int http11_header_accept_language(httpaa *http,
        stringaa *request)
{
	int is_accept_language = 0;

	while (1)
	{
		if (http11_language_range(request))
		{
			if (stringaa_is_char(request, ';'))
			{
				if (stringaa_is_chari(request, 'q'))
				{
					if (stringaa_is_char(request, '='))
					{
						if (http11_qvalue(request))
						{
						}
					}
				}
			}
		}
	}

	return is_accept_language;
}

HTTPAA_INLINE int http11_language_range(stringaa *request)
{
	char c = 0;
	int is_language_range = 0;
	size_t pos = 0;
	int i = 0;

	pos = request->read_pos;

	if (stringaa_is_char(request, '*'))
	{
	}
	else
	{
		c = stringaa_get_char(request);
		for (i = 0; i < 8; i++)
		{
			if (HTTP11_DIGIT)
			{
				stringaa_next_read_pos(request);
				c = stringaa_get_char(request);
			}
			else
			{
				break;
			}
		}

		if (stringaa_is_char(request, '-'))
		{
			for (i = 0; i < 8; i++)
			{
				if (HTTP11_DIGIT)
				{
					stringaa_next_read_pos(request);
					c = stringaa_get_char(request);
				}
				else
				{
					break;
				}
			}
		}
	}

	if (is_language_range)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_language_range;
}

HTTPAA_INLINE int http11_header_authorization(httpaa *http,
        stringaa *request)
{
	int is_authorization = 0;

	if (http11_credentials(request))
	{
	}

	return is_authorization;
}

HTTPAA_INLINE int http11_credentials(stringaa *request)
{
	return 0;
}

HTTPAA_INLINE int http11_header_expect(httpaa *http,
        stringaa *request)
{
	int is_expect = 0;

	while (1)
	{
		if (http11_expectation(request))
		{
		}
		if (http11_lws(request))
		{
		}
		if (stringaa_is_char(request, ','))
		{
		}
		if (http11_lws(request))
		{
		}
	}

	return is_expect;
}

HTTPAA_INLINE int http11_expectation(stringaa *request)
{
	char c = 0;
	int is_expectation = 0;
	const char *s_100_continue = "100-continue";
	size_t pos = 0;
	const char *pc = NULL;

	pos = request->read_pos;
	c = stringaa_get_char(request);

	for (pc = s_100_continue; *pc != 0; pc++)
	{
		if (*pc == _tolower(c))
		{
			stringaa_next_read_pos(request);
			c = stringaa_get_char(request);
		}
		else
		{
			break;
		}
	}
	if (*pc == 0)
	{
		is_expectation = 1;
	}

	if (is_expectation)
	{
	}
	else
	{
		if (http11_expectation_extension(request))
		{
		}
		else
		{
			request->read_pos = pos;
		}
	}

	return is_expectation;
}

HTTPAA_INLINE int http11_expectation_extension(stringaa *request)
{
	int is_expectation_extension = 0;

	if (http11_token(request))
	{
		if (stringaa_is_char(request, '='))
		{
			if (http11_token(request))
			{
			}
			else if (http11_quoted_string(request))
			{
			}
			while (1)
			{
				if (http11_expect_params(request))
				{
				}
				else
				{
					break;
				}
			}
		}
	}

	return is_expectation_extension;
}

HTTPAA_INLINE int http11_expect_params(stringaa *request)
{
	int is_expect_params = 0;

	if (stringaa_is_char(request, ';'))
	{
		if (http11_token(request))
		{
			if (stringaa_is_char(request, '='))
			{
				if (http11_token(request))
				{
				}
				else if (http11_quoted_string(request))
				{
				}
			}
		}
	}

	return is_expect_params;
}

HTTPAA_INLINE int http11_header_from(httpaa *http,
        stringaa *request)
{
	int is_from = 0;

	if (http11_mailbox(request))
	{
	}

	return is_from;
}

HTTPAA_INLINE int http11_mailbox(stringaa *request)
{
	return 0;
}

HTTPAA_INLINE int http11_header_host(httpaa *http,
        stringaa *request)
{
	int is_host = 0;

	if (http11_host(request))
	{
		is_host = 1;
		if (stringaa_is_char(request, ':'))
		{
			if (http11_port(request))
			{
			}
		}
	}

	return is_host;
}

HTTPAA_INLINE int http11_header_if_match(httpaa *http,
        stringaa *request)
{
	int is_if_match = 0;

	if (stringaa_is_char(request, '*'))
	{
	}
	else
	{
		while (1)
		{
			if (http11_entity_tag(request))
			{
			}
		}
	}

	return is_if_match;
}

HTTPAA_INLINE int http11_entity_tag(stringaa *request)
{
	int is_entity_tag = 0;

	if (stringaa_is_chari(request, 'w'))
	{
		if (stringaa_is_char(request, '/'))
		{
		}
	}
	if (http11_quoted_string(request))
	{
	}

	return is_entity_tag;
}

HTTPAA_INLINE int http11_header_if_modified_since(httpaa *http,
        stringaa *request)
{
	int is_if_modified_since = 0;

	if (http11_date(request))
	{
		is_if_modified_since = 1;
	}

	return is_if_modified_since;
}

HTTPAA_INLINE int http11_header_if_none_match(httpaa *http,
        stringaa *request)
{
	int is_if_none_match = 0;

	if (stringaa_is_char(request, '*'))
	{
	}
	else
	{
		while (1)
		{
			if (http11_entity_tag(request))
			{
			}
		}
	}

	return is_if_none_match;
}

HTTPAA_INLINE int http11_header_if_range(httpaa *http,
        stringaa *request)
{
	int is_if_range = 0;

	if (http11_entity_tag(request))
	{
	}
	else if (http11_date(request))
	{
	}

	return is_if_range;
}

HTTPAA_INLINE int http11_header_if_unmodified_since(httpaa *http,
        stringaa *request)
{
	int is_if_unmodified_since = 0;

	if (http11_date(request))
	{
	}

	return is_if_unmodified_since;
}

HTTPAA_INLINE int http11_header_max_forwards(httpaa *http,
        stringaa *request)
{
	char c = 0;
	int is_max_forwards = 0;
	size_t pos = 0;

	pos = request->read_pos;

	c = stringaa_get_char(request);

	if (HTTP11_DIGIT)
	{
		stringaa_next_read_pos(request);
	}

	return is_max_forwards;
}

HTTPAA_INLINE int http11_header_proxy_authorization(httpaa *http,
        stringaa *request)
{
	int is_proxy_authorization = 0;

	if (http11_credentials(request))
	{
	}

	return is_proxy_authorization;
}

HTTPAA_INLINE int http11_header_range(httpaa *http,
        stringaa *request)
{
	int is_range = 0;

	if (http11_bytes_unit(request))
	{
		if (stringaa_is_char(request, '='))
		{
			if (http11_byte_range_set(request))
			{
				is_range = 1;
			}
		}
	}

	return is_range;
}

HTTPAA_INLINE int http11_bytes_unit(stringaa *request)
{
	char c = 0;
	int is_bytes_unit = 0;
	size_t pos = 0;
	const char *s_bytes = "bytes";
	const char *pc = NULL;

	pos = request->read_pos;

	c = stringaa_get_char(request);

	for (pc = s_bytes; *pc != 0; pc++)
	{
		if (*pc == _tolower(c))
		{
			stringaa_next_read_pos(request);
			c = stringaa_get_char(request);
		}
		else
		{
			break;
		}
	}
	if (*pc == 0)
	{
		is_bytes_unit = 1;
	}

	return is_bytes_unit;
}

HTTPAA_INLINE int http11_byte_range_set(stringaa *request)
{
	int is_byte_range_set = 0;

	while (1)
	{
		if (http11_byte_range_spec(request))
		{
		}
		else if (http11_suffix_byte_range_spec(request))
		{
		}
		else
		{
			break;
		}
	}

	return is_byte_range_set;
}

HTTPAA_INLINE int http11_byte_range_spec(stringaa *request)
{
	char c = 0;
	int is_byte_range_spec = 0;
	size_t pos = 0;

	pos = request->read_pos;

	c = stringaa_get_char(request);

	if (HTTP11_DIGIT)
	{
		stringaa_next_read_pos(request);
		if (stringaa_is_char(request, '-'))
		{
			c = stringaa_get_char(request);
			if (HTTP11_DIGIT)
			{
				stringaa_next_read_pos(request);
				is_byte_range_spec = 1;
			}
		}
	}

	return is_byte_range_spec;
}

HTTPAA_INLINE int http11_suffix_byte_range_spec(stringaa *request)
{
	char c = 0;
	int is_suffix_byte_range_spec = 0;
	size_t pos = 0;

	pos = request->read_pos;

	if (stringaa_is_char(request, '-'))
	{
		c = stringaa_get_char(request);
		if (HTTP11_DIGIT)
		{
			stringaa_next_read_pos(request);
			is_suffix_byte_range_spec = 1;
		}
	}

	return is_suffix_byte_range_spec;
}

HTTPAA_INLINE int http11_header_referer(httpaa *http,
        stringaa *request)
{
	int is_referer = 0;
	struct uri_struct uri = {{0}};

	if (absoluteURI(&uri))
	{
	}
	else if (relativeURI(&uri))
	{
	}

	return is_referer;
}

HTTPAA_INLINE int http11_header_te(httpaa *http,
        stringaa *request)
{
	int is_te = 0;

	while (1)
	{
		if (http11_t_codings(http, request))
		{
		}
		else
		{
			break;
		}
	}

	return is_te;
}

HTTPAA_INLINE int http11_t_codings(httpaa *http,
		stringaa *request)
{
	char c = 0;
	int is_t_codings = 0;
	size_t pos = 0;
	const char *s_trailers = "trailers";
	const char *pc = NULL;

	pos = request->read_pos;

	c = stringaa_get_char(request);

	for (pc = s_trailers; *pc != 0; pc++)
	{
		if (*pc == _tolower(c))
		{
			stringaa_next_read_pos(request);
			c = stringaa_get_char(request);
		}
		else
		{
			break;
		}
	}
	if (*pc == 0)
	{
		is_t_codings = 1;
	}

	if (!is_t_codings)
	{
		if (http11_header_transfer_extension(http, request))
		{
			is_t_codings = 1;
			if (http11_accept_params(request))
			{
			}
		}
	}

	if (is_t_codings)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_t_codings;
}

HTTPAA_INLINE int http11_header_user_agent(httpaa *http,
        stringaa *request)
{
	int is_user_agent = 0;

	while (1)
	{
		if (http11_product(request))
		{
		}
		else if (http11_comment(request))
		{
		}
	}

	return is_user_agent;
}

HTTPAA_INLINE int http11_header_accept_ranges(httpaa *http,
        stringaa *request)
{
	char c = 0;
	int is_accept_ranges = 0;
	size_t pos = 0;
	const char *s_none = "none";
	const char *pc = NULL;

	pos = request->read_pos;

	while (1)
	{
		if (http11_range_unit(request))
		{
		}
		else
		{
			break;
		}
	}

	if (!is_accept_ranges)
	{
		c = stringaa_get_char(request);
		for (pc = s_none; *pc != 0; pc++)
		{
			if (*pc == _tolower(c))
			{
				stringaa_next_read_pos(request);
				c = stringaa_get_char(request);
			}
			else
			{
				break;
			}
		}
		if (*pc == 0)
		{
			is_accept_ranges = 1;
		}
	}

	return is_accept_ranges;
}

HTTPAA_INLINE int http11_range_unit(stringaa *request)
{
	int is_range_unit = 0;
	size_t pos = 0;

	pos = request->read_pos;

	if (http11_bytes_unit(request))
	{
		is_range_unit = 1;
	}
	else if (http11_token(request))
	{
		is_range_unit = 1;
	}

	if (is_range_unit)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_range_unit;
}

HTTPAA_INLINE int http11_header_age(httpaa *http,
        stringaa *request)
{
	int is_age = 0;

	if (http11_delta_seconds(request))
	{
		is_age = 1;
	}

	return is_age;
}

HTTPAA_INLINE int http11_header_etag(httpaa *http,
        stringaa *request)
{
	int is_etag = 0;

	if (http11_entity_tag(request))
	{
	}

	return is_etag;
}

HTTPAA_INLINE int http11_header_location(httpaa *http,
        stringaa *request)
{
	int is_location = 0;
	struct uri_struct uri = {{0}};

	if (absoluteURI(&uri))
	{
		is_location = 1;
	}

	return is_location;
}

HTTPAA_INLINE int http11_header_proxy_authenticate(httpaa *http,
        stringaa *request)
{
	int is_proxy_authenticate = 0;

	while (1)
	{
		if (http11_challenge(request))
		{
		}
	}

	return is_proxy_authenticate;
}

HTTPAA_INLINE int http11_challenge(stringaa *request)
{
	return 0;
}

HTTPAA_INLINE int http11_header_retry_after(httpaa *http,
        stringaa *request)
{
	int is_retry_after = 0;

	if (http11_date(request))
	{
	}
	else if (http11_delta_seconds(request))
	{
	}

	return is_retry_after;
}

HTTPAA_INLINE int http11_header_server(httpaa *http,
        stringaa *request)
{
	int is_server = 0;

	while (1)
	{
		if (http11_product(request))
		{
		}
		else if (http11_comment(request))
		{
		}
	}

	return is_server;
}

HTTPAA_INLINE int http11_header_vary(httpaa *http,
        stringaa *request)
{
	int is_vary = 0;

	if (stringaa_is_char(request, '*'))
	{
	}
	else
	{
		while (1)
		{
			if (http11_field_name(http, request))
			{
			}
			else
			{
				break;
			}
		}
	}

	return is_vary;
}

HTTPAA_INLINE int http11_header_www_authenticate(httpaa *http,
        stringaa *request)
{
	int is_www_authenticate = 0;

	if (http11_challenge(request))
	{
	}

	return is_www_authenticate;
}

HTTPAA_INLINE int http11_header_allow(httpaa *http,
        stringaa *request)
{
	char c = 0;
	int is_allow = 0;
	int i = 0;
	size_t pos = 0;
	const char *pc = NULL;
	int is_method = 0;

	pos = request->read_pos;

	while (1)
	{
		c = stringaa_get_char(request);

		for (i = 0; i < HTTP11_METHOD_ARRAY_SIZE; i++)
		{
			for (pc = http11_method_array[i]; *pc != 0; pc++)
			{
				if (*pc == _tolower(c))
				{
					stringaa_next_read_pos(request);
					c = stringaa_get_char(request);
				}
				else
				{
					break;
				}
			}
			if (*pc == 0)
			{
				is_method = 1;
				break;
			}
		}

		if (is_method)
		{
			is_allow = 1;
			is_method = 0;
			if (http11_lws(request))
			{
			}
			if (stringaa_is_char(request, ','))
			{
			}
			if (http11_lws(request))
			{
			}
		}
		else
		{
			break;
		}
	}

	if (is_allow)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_allow;
}

HTTPAA_INLINE int http11_header_content_encoding(httpaa *http,
        stringaa *request)
{
	int is_content_encoding = 0;

	while (1)
	{
		if (http11_content_coding(request))
		{
		}
	}

	return is_content_encoding;
}

HTTPAA_INLINE int http11_header_content_language(httpaa *http,
        stringaa *request)
{
	int is_content_language = 0;

	while (1)
	{
		if (http11_language_tag(request))
		{
		}
	}

	return is_content_language;
}

HTTPAA_INLINE int http11_language_tag(stringaa *request)
{
	char c = 0;
	int is_language_tag = 0;
	size_t pos = 0;
	int i = 0;

	pos = request->read_pos;

	c = stringaa_get_char(request);
	for (i = 0; i < 8; i++)
	{
		if (HTTP11_DIGIT)
		{
			stringaa_next_read_pos(request);
			c = stringaa_get_char(request);
		}
		else
		{
			break;
		}
	}

	if (request->read_pos != pos)
	{
		is_language_tag = 1;
	}

	if (is_language_tag)
	{
		if (stringaa_is_char(request, '-'))
		{
			for (i = 0; i < 8; i++)
			{
				if (HTTP11_DIGIT)
				{
					stringaa_next_read_pos(request);
					c = stringaa_get_char(request);
				}
				else
				{
					break;
				}
			}
		}
	}

	if (is_language_tag)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_language_tag;
}

HTTPAA_INLINE int http11_header_content_length(httpaa *http,
        stringaa *request)
{
	char c = 0;
	int is_content_length = 0;
	size_t pos = 0;

	pos = request->read_pos;

	c = stringaa_get_char(request);

	while (1)
	{
		if (HTTP11_DIGIT)
		{
			stringaa_next_read_pos(request);
			c = stringaa_get_char(request);
		}
		else
		{
			break;
		}
	}

	if (request->read_pos != pos)
	{
		is_content_length = 1;
	}

	return is_content_length;
}

HTTPAA_INLINE int http11_header_content_location(httpaa *http,
        stringaa *request)
{
	int is_content_location = 0;
	struct uri_struct uri = {{0}};

	if (absoluteURI(&uri))
	{
	}
	else if (relativeURI(&uri))
	{
	}

	return is_content_location;
}

HTTPAA_INLINE int http11_header_content_md5(httpaa *http,
        stringaa *request)
{
	return 0;
}

HTTPAA_INLINE int http11_header_content_range(httpaa *http,
        stringaa *request)
{
	char c = 0;
	int is_content_range = 0;
	size_t pos = 0;
	int is_byte_range_resp_spec = 0;
	size_t digit_pos = 0;

	pos = request->read_pos;

	c = stringaa_get_char(request);

	if (http11_bytes_unit(request))
	{
		if (HTTP11_SP)
		{
			stringaa_next_read_pos(request);
			if (stringaa_is_char(request, '*'))
			{
				is_byte_range_resp_spec = 1;
			}
			else
			{
				digit_pos = request->read_pos;
				c = stringaa_get_char(request);
				while (1)
				{
					if (HTTP11_DIGIT)
					{
						stringaa_next_read_pos(request);
						c = stringaa_get_char(request);
					}
					else
					{
						break;
					}
				}
				if (request->read_pos != digit_pos)
				{
					if (stringaa_is_char(request, '-'))
					{
						digit_pos = request->read_pos;
						c = stringaa_get_char(request);
						while (1)
						{
							if (HTTP11_DIGIT)
							{
								stringaa_next_read_pos(request);
								c = stringaa_get_char(request);
							}
							else
							{
								break;
							}
						}
						if (request->read_pos != digit_pos)
						{
							is_byte_range_resp_spec = 1;
						}
					}
				}
			}
			if (is_byte_range_resp_spec)
			{
				if (stringaa_is_char(request, '/'))
				{
					if (stringaa_is_char(request, '*'))
					{
					}
					else
					{
						c = stringaa_get_char(request);
						while (1)
						{
							if (HTTP11_DIGIT)
							{
								stringaa_next_read_pos(request);
								c = stringaa_get_char(request);
							}
							else
							{
								break;
							}
						}
					}
				}
			}
		}
	}

	is_content_range = is_byte_range_resp_spec;

	if (is_content_range)
	{
	}
	else
	{
		request->read_pos = pos;
	}

	return is_content_range;
}

HTTPAA_INLINE int http11_header_content_type(httpaa *http,
        stringaa *request)
{
	int is_content_type = 0;

	if (http11_media_type(request))
	{
	}

	return is_content_type;
}

HTTPAA_INLINE int http11_media_type(stringaa *request)
{
	int is_media_type = 0;

	if (http11_token(request))
	{
		if (stringaa_is_char(request, '/'))
		{
			if (http11_token(request))
			{
				while (1)
				{
					if (stringaa_is_char(request, ';'))
					{
						if (http11_parameter(request))
						{
						}
					}
				}
			}
		}
	}

	return is_media_type;
}

HTTPAA_INLINE int http11_header_expires(httpaa *http,
        stringaa *request)
{
	int is_expires = 0;

	if (http11_date(request))
	{
		is_expires = 1;
	}

	return is_expires;
}

HTTPAA_INLINE int http11_header_last_modified(httpaa *http,
        stringaa *request)
{
	int is_last_modified = 0;

	if (http11_date(request))
	{
		is_last_modified = 1;
	}

	return is_last_modified;
}

#endif	/* HTTPAA_H */

