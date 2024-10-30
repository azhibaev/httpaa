/* HTTP/1.1 RFC 2616
 * Copyright (C) 2021  Askar Azhibaev
 *
 * File: httpaa_work.h
 * Description: Implementation of HTTP/1.1 RFC 2616
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: December 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef HTTPAA_WORK_H
#define HTTPAA_WORK_H

HTTPAA_INLINE int httpaa_work_get_request_file_name(httpaa *http)
{
	int is_get = 0;

	if (http)
	{
		if (stringaa_copy(&http->request_file_name, &http->request.request_line.request_uri.abs_path))
		{
			if (uriaa_remove_segment_point(&http->request_file_name))
			{
				if (uriaa_remove_segment_points(&http->request_file_name))
				{
					is_get = 1;
				}
			}
		}
	}

	return is_get;
}

HTTPAA_INLINE int httpaa_work_set_response_file_name(httpaa *http, stringaa *path)
{
	int is_set = 0;

	if (http)
	{
		if (stringaa_copy(&http->response_file_name, path))
		{
			if (stringaa_copy(&http->response_file_name, &http->request_file_name))
			{
				if (uriaa_remove_segment_point(&http->response_file_name))
				{
					if (uriaa_remove_segment_points(&http->response_file_name))
					{
						is_set = 1;
					}
				}
			}
		}
	}

	return is_set;
}

/* Method GET
 * verify file name
 * open file
 * read file to buffer
 * return
 */
HTTPAA_INLINE int http11_work_method_get(httpaa *http, stringaa *path)
{
	int is_set = 0;

	if (http)
	{
		if (httpaa_work_get_request_file_name(http))
		{
			if (httpaa_work_set_response_file_name(http, path))
			{
				is_set = 1;
			}
		}
	}

	return is_set;
}

#endif	/* HTTPAA_WORK_H */

