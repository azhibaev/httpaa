/* Worker library. Pool of pthread workers.
 * Copyright (C) 2021  Askar Azhibaev
 * 
 * File: workeraa_http.h
 * Description: Worker HTTP
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: February 2021
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef __WORKERAA_HTTP_H
#define __WORKERAA_HTTP_H

/* Worker for http calls Http */
#include "httpaa.h"

WORKERAA_INLINE int workeraa_http_init(workeraa *worker);

typedef struct
{
	httpaa http;
	stringaa request;		/* request buffer */
	stringaa response;		/* response buffer */
	stringaa path;
	union
	{
		unsigned int flags;
		struct
		{
			unsigned int is_header_recv : 1;	/* 1 if http header is parsed */
			unsigned int is_message_recv : 1;	/* 1 if receive http message */
			unsigned int is_header_send : 1;	/* 1 if http header is parsed */
			unsigned int is_message_send : 1;	/* 1 if receive http message */
			unsigned int is_init : 1;			/* 1 if buffers is init */
		};
	};
} workeraa_http;

/* Recv data and find http header
 * in: workeraa_http.request
 * out: is_header
 * 1. socketaa_worker reads data from network socket
 *    to request buffer of workeraa_http,
 * 2. socketaa_worker calls workeraa 'recv' callback with data,
 * 3. workeraa_http parse request headers with httpaa
 * 4. workeraa_http returns state to socketaa_worker
 */
WORKERAA_INLINE int workeraa_http_header(workeraa *worker)
{
	int is_header = 0;
	workeraa_http *http = NULL;

	if (worker && worker->work.p_struct)
	{
		http = (workeraa_http*)worker->work.p_struct;
		if (http->is_header_recv == 0)
		{
			if (httpaa_parse_request_headers(&http->http, &http->request))
			{
				http->is_header_recv = 1;
			}
		}
		is_header = http->is_header_recv;
	}

	return is_header;
}

/* Recv http message
 * in: workeraa_http.request
 * out: is_message
 */
WORKERAA_INLINE int workeraa_http_message(workeraa *worker)
{
	int is_message = 0;
	workeraa_http *http = NULL;

	if (worker && worker->work.p_struct)
	{
		http = (workeraa_http*)worker->work.p_struct;
		if (http->is_message_recv == 0)
		{
		}
	}

	return is_message;
}

WORKERAA_INLINE int workeraa_http_create_response(workeraa *worker)
{
	int is_create = 0;
	workeraa_http *worker_http = NULL;
	char buf[64] = {0};

	if (worker)
	{
		if (worker->work.p_struct)
		{
			worker_http = (workeraa_http*)worker->work.p_struct;
			worker_http->http.response.status_line.http_version.major_version = 1;
			worker_http->http.response.status_line.http_version.minor_version = 1;
			stringaa_clean(&worker_http->http.response.status_line.reason_phrase);
			stringaa_add_chars(&worker_http->http.response.status_line.reason_phrase, http11_status_code_array[http11_status_code_key_200_OK * 2]);
			stringaa_add_chars(&worker_http->http.response.status_line.reason_phrase, " ");
			stringaa_add_chars(&worker_http->http.response.status_line.reason_phrase, http11_status_code_array[http11_status_code_key_200_OK * 2 + 1]);
			stringaa_set_chars(&worker_http->http.response.message_body.entity_body, "<html><head><title>Hello</title></head><body><h1>Hello, World!</h1></body></html>");

			stringaa_clean(&worker_http->response);
			sprintf(buf, "HTTP/%d.%d ", worker_http->http.response.status_line.http_version.major_version, worker_http->http.response.status_line.http_version.minor_version);
			stringaa_add_chars(&worker_http->response, buf);
			stringaa_add(&worker_http->response, &worker_http->http.response.status_line.reason_phrase);
			stringaa_add_chars(&worker_http->response, "\r\n");
			stringaa_add_chars(&worker_http->response, http11_entity_header_original_array[http11_entity_header_content_length - HTTP11_ENTITY_HEADER_ARRAY_START]);
			sprintf(buf, ": %zu\r\n", worker_http->http.response.message_body.entity_body.write_pos);
			stringaa_add_chars(&worker_http->response, buf);
			stringaa_add_chars(&worker_http->response, "\r\n");
			stringaa_add(&worker_http->response, &worker_http->http.response.message_body.entity_body);
			is_create = 1;
		}
	}

	return is_create;
}

WORKERAA_INLINE int workeraa_http_method(workeraa *worker, enum http11_method_index method)
{
	int is_set = 0;
	workeraa_http *worker_http = NULL;

	if (worker)
	{
		if (worker->work.p_struct)
		{
			worker_http = (workeraa_http*)worker->work.p_struct;
			switch (method)
			{
				case http11_method_get:
					is_set = http11_work_method_get(&worker_http->http, &worker_http->path);
					break;
				case http11_method_head:
					break;
				case http11_method_post:
					break;
				case http11_method_put:
					break;
				case http11_method_delete:
					break;
				case http11_method_trace:
					break;
				case http11_method_connect:
					break;
				default:
					is_set = 0;
					break;
			}
		}
	}

	return is_set;
}

/* Callback link for http
 * recv data from worker 
 * call by workeraa_http_func with WORKERAA_FUNC_RECV
 * 1. recv HTTP request
 * 2. parse request */
WORKERAA_INLINE int workeraa_http_recv(workeraa *worker)
{
	int is_link = 0;
	workeraa_http *worker_http = NULL;

	if (worker)
	{
		if (worker->work.p_struct)
		{
			worker_http = (workeraa_http*)worker->work.p_struct;
			__WORKERAA_DEBUG(stringaa_fprint_line(stderr, &worker_http->request););
			if (workeraa_http_header(worker))
			{
				is_link = 1;
				if (workeraa_http_method(worker, worker_http->http.request.request_line.method))
				{
				}
				if (worker_http->http.request.request_line.method == http11_method_get)
				{
					/* If GET method then set SEND flag */
					if (socketaa_set_work_flags(&worker->work, SOCKETAA_WORK_FLAGS_IS_SEND | SOCKETAA_WORK_FLAGS_IS_SELECT))
					{
					}
				}
			}
			else if (workeraa_http_message(worker))
			{
				is_link = 1;
			}
		}
	}

	return is_link;
}

/* Callback link for http
 * send data to worker 
 * call by workeraa_http_func with WORKERAA_FUNC_SEND */
WORKERAA_INLINE int workeraa_http_send(workeraa *worker)
{
	int is_link = 0;
	workeraa_http *worker_http = NULL;

	if (worker)
	{
		if (worker->work.p_struct)
		{
			worker_http = (workeraa_http*)worker->work.p_struct;
			if (worker_http->is_header_send == 0)
			{
				if (workeraa_http_create_response(worker))
				{
					worker_http->is_header_send = 1;
				}
			}
		}
	}

	return is_link;
}

/* Free workeraa_http struct
 * call by workeraa_http_func with WORKERAA_FUNC_FREE
 * in: workeraa_http.{request,response}
 * out: is_free
 */
WORKERAA_INLINE int workeraa_http_free(workeraa *worker)
{
	int is_free = 0;

	if (worker)
	{
		if (worker->work.p_struct)
		{
			free((workeraa_http*)worker->work.p_struct);
			worker->work.p_struct = NULL;
		}
		is_free = 1;
	}

	return is_free;
}

/* Init http worker link
 * call by workeraa_http_func with WORKERAA_FUNC_INIT
 * in: worker_http_struct.is_init
 * out: worker_http_struct.{p_recv,p_send}
 * Worker http has 'recv' and 'send' buffers
 * Socket worker has pointers to Worker's http 'recv' and 'send' buffers
 * Socket worker takes pointers from workeraa struct */
WORKERAA_INLINE int workeraa_http_init(workeraa *worker)
{
	int is_init = 0;
	workeraa_http *worker_http = NULL;

	if (worker)
	{
		if (worker->work.p_struct == NULL)
		{
			worker->work.p_struct = (void*) malloc(sizeof(workeraa_http));
			if (worker->work.p_struct)
			{
				memset(worker->work.p_struct, 0, sizeof(workeraa_http));
			}
		}
		if (worker->work.p_struct)
		{
			worker_http = (workeraa_http*)worker->work.p_struct;
			if (worker_http->request.is_init == 0)
			{
				if (stringaa_malloc(&worker_http->request, WORKERAA_WORK_RECV_BUFFER_SIZE_DEFAULT))
				{
					worker_http->request.is_init = 1;
				}
			}
			else
			{
				stringaa_clean(&worker_http->request);
			}
			if (worker_http->response.is_init == 0)
			{
				if (stringaa_malloc(&worker_http->response, WORKERAA_WORK_SEND_BUFFER_SIZE_DEFAULT))
				{
					worker_http->response.is_init = 1;
				}
			}
			else
			{
				stringaa_clean(&worker_http->response);
			}
		}
		is_init = 1;
	}

	return is_init;
}

WORKERAA_INLINE int workeraa_http_get_buffer(workeraa *worker,
		enum workeraa_func_enum buffer_id)
{
	int is_get = 0;
	workeraa_http *worker_http = NULL;

	if (worker)
	{
		if (worker->work.p_struct)
		{
			worker_http = (workeraa_http*)worker->work.p_struct;
			switch (buffer_id)
			{
				case WORKERAA_FUNC_GET_RECV_BUFFER:
					worker->work.p_data = (void*)&worker_http->request;
					is_get = 1;
					break;
				case WORKERAA_FUNC_GET_SEND_BUFFER:
					worker->work.p_data = (void*)&worker_http->response;
					is_get = 1;
					break;
				default:
					break;
			}
		}
	}

	return is_get;
}

/* Entry point for functions:
 * workeraa_http_init, 
 * workeraa_http_recv, 
 * workeraa_http_send, 
 * workeraa_http_free */
WORKERAA_INLINE int workeraa_http_func(workeraa *worker, 
		enum workeraa_func_enum func_id)
{
	int is_func = 0;

	switch (func_id)
	{
		case WORKERAA_FUNC_INIT:
			is_func = workeraa_http_init(worker);
			break;
		case WORKERAA_FUNC_RECV_DATA:
			is_func = workeraa_http_recv(worker);
			break;
		case WORKERAA_FUNC_SEND_DATA:
			is_func = workeraa_http_send(worker);
			break;
		case WORKERAA_FUNC_GET_BUFFER:
		case WORKERAA_FUNC_GET_RECV_BUFFER:
		case WORKERAA_FUNC_GET_SEND_BUFFER:
			is_func = workeraa_http_get_buffer(worker, func_id);
			break;
		case WORKERAA_FUNC_FREE:
			is_func = workeraa_http_free(worker);
			break;
		default:
			break;
	}

	return is_func;
}

#endif	// __WORKERAA_HTTP_H

