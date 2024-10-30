/* URI RFC 2396 Library.
 * Copyright (C) 2021  Askar Azhibaev
 *
 * File: uriaa_rfc2396_resolve.h
 * Description: Implementation of URI RFC 2396.
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: November 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef URIAA_RFC2396_RESOLVE_H
#define URIAA_RFC2396_RESOLVE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stringaa.h"
#include "uriaa_structs.h"
#include "uriaa_rfc2396.h"

#define URIAA_RESULT_IS_CURRENT_DOCUMENT 2
#define URIAA_RESULT_IS_ABSOLUTE_URI 3

URIAA_INLINE int uriaa_resolve_uri(uriaa *relative_uri, uriaa *base_uri)
{
	int is_network_path = 0;
	int is_absolute_path = 0;
	size_t i_s = 0;
	size_t i_e = 0;
	size_t base_uri_path_copy_size = 0;
	size_t buf_size = 0;
	char *buf = NULL;
	size_t segment_s = 0;
	size_t relative_uri_path_copy_size = 0;
	size_t read_pos = 0;
	size_t buf_copy_size = 0;

	if (relative_uri->net_path.is_init == 0 && 
			relative_uri->abs_path.is_init == 0 && 
			relative_uri->rel_path.is_init == 0 && 
			relative_uri->opaque_part.is_init == 0)
	{
		if (relative_uri->scheme.is_init == 0)
		{
			if (relative_uri->authority.is_init == 0)
			{
				if (relative_uri->query.is_init == 0)
				{
					return URIAA_RESULT_IS_CURRENT_DOCUMENT;
				}
			}
		}
	}

	if (relative_uri->scheme.is_init)
	{
		return URIAA_RESULT_IS_ABSOLUTE_URI;
	}
	else
	{
		relative_uri->scheme = base_uri->scheme;
		relative_uri->scheme.is_alloc = 0;
		relative_uri->scheme.is_link = 1;
	}

	if (relative_uri->authority.is_init)
	{
		is_network_path = 1;
	}
	else
	{
		relative_uri->authority = base_uri->authority;
		relative_uri->authority.is_alloc = 0;
		relative_uri->authority.is_link = 1;
	}

	if (!is_network_path)
	{
		if (relative_uri->abs_path.is_init && relative_uri->rel_path.is_init == 0)
		{
			is_absolute_path = 1;
		}
		else if (relative_uri->opaque_part.is_init)
		{
			if (relative_uri->opaque_part.buf && relative_uri->opaque_part.buf[0] == '/')
			{
				is_absolute_path = 1;
			}
		}
	}

	if (!is_network_path && !is_absolute_path)
	{
		if (base_uri->abs_path.is_init)
		{
			if (base_uri->abs_path.buf && base_uri->abs_path.write_pos > 0)
			{
				i_s = base_uri->abs_path.write_pos - 1;
				if (i_s > 0 && base_uri->abs_path.is_null_terminated)
				{
					i_s--;
				}
				i_e = 0;
				while (i_s >= i_e)
				{
					if (base_uri->abs_path.buf[i_s] == '/')
					{
						base_uri_path_copy_size = i_s + 1;
						break;
					}
					i_s--;
				}
				if (base_uri_path_copy_size == 0)
				{
					base_uri_path_copy_size = base_uri->abs_path.write_pos;
				}
			}
		}

		if (relative_uri->rel_path.is_init)
		{
			if (relative_uri->rel_path.buf)
			{
				relative_uri_path_copy_size = relative_uri->rel_path.write_pos;
				if (relative_uri_path_copy_size > 0 && relative_uri->rel_path.is_null_terminated)
				{
					relative_uri_path_copy_size--;
				}
			}
		}

		buf_size = base_uri_path_copy_size + relative_uri_path_copy_size + 1;
		buf = (char*) malloc(buf_size);

		if (buf)
		{
			if (base_uri->abs_path.buf && base_uri_path_copy_size)
			{
				memcpy(buf, base_uri->abs_path.buf, base_uri_path_copy_size);
			}
			if (relative_uri->rel_path.buf && relative_uri_path_copy_size)
			{
				memcpy(buf + base_uri_path_copy_size, relative_uri->rel_path.buf, 
						relative_uri_path_copy_size);
			}
			buf[buf_size - 1] = 0;
		}
		else
		{
			return 0;
		}

		if (relative_uri->path.buf && relative_uri->path.is_alloc)
		{
			free(relative_uri->path.buf);
		}

		relative_uri->path.buf = buf;
		relative_uri->path.buf_size = buf_size;
		relative_uri->path.read_pos = 0;
		relative_uri->path.flags = STRINGAA_IS_INIT |
			STRINGAA_IS_ALLOC | STRINGAA_IS_NULL_TERMINATED;
		relative_uri->path.write_pos = buf_size ? buf_size - relative_uri->path.is_null_terminated : 0;

		buf_size--;
		i_s = 0;

		while(1)
		{
			while (i_s < buf_size)
			{
				if (buf[i_s] == '/')
				{
					if (i_s - segment_s == 1 && buf[i_s - 1] == '.')
					{
						memmove(buf + segment_s, buf + i_s + 1, buf_size - i_s);
						buf_size -= i_s + 1 - segment_s;
						i_s = segment_s;
						printf("buf: %s\n", buf);
						break;
					}
					segment_s = i_s + 1;
				}
				i_s++;
			}
			if (i_s >= buf_size)
			{
				break;
			}
		}
		if (segment_s + 1 == buf_size && buf[segment_s] == '.')
		{
			buf[segment_s] = 0;
		}

		i_s = 0;

		while (1)
		{
			segment_s = 0;

			while (i_s < buf_size)
			{
				if (buf[i_s] == '/')
				{
					/* segment is '../' */
					if (i_s - segment_s == 2 && buf[i_s - 1] == '.' && buf[i_s - 2] == '.')
					{
						segment_s = i_s + 1;
					}
					/* segment is '<segment>/..' */
					else if (i_s + 2 < buf_size && buf[i_s + 1] == '.' && buf[i_s + 2] == '.')
					{
						/* segment is '<segment>/../' */
						if (i_s + 3 < buf_size && buf[i_s + 3] == '/')
						{
							memmove(buf + segment_s, buf + i_s + 4, buf_size - i_s - 4 + 1);
							buf_size -= i_s + 4 - segment_s;
							i_s = 0;
							printf("buf: %s\n", buf);
							break;
						}
						/* segment is '<segment>/..$' */
						else if (i_s + 3 == buf_size)
						{
							memmove(buf + segment_s, buf + i_s + 3, buf_size - i_s - 3 + 1);
							buf_size -= i_s + 3 - segment_s;
							i_s = 0;
							printf("buf: %s\n", buf);
							break;
						}
					}
					/* segment is '<segment>/<segment>' */
					else
					{
						segment_s = i_s + 1;
					}
				}
				i_s++;
			}
			if (i_s >= buf_size)
			{
				break;
			}
		}

		relative_uri->path.buf_size = buf_size + 1;
		relative_uri->path.write_pos = buf_size + 1 - relative_uri->path.is_null_terminated;

		segment_s = 0;

		for (i_s = 0; i_s < buf_size; i_s++)
		{
			if (buf[i_s] == '/')
			{
				if (i_s - segment_s == 2 && buf[i_s - 1] == '.' && buf[i_s - 2] == '.')
				{
					return 0;
				}
				else if (i_s - segment_s > 0)
				{
					break;
				}
				segment_s = i_s + 1;
			}
		}
		if (segment_s + 2 == buf_size && buf[segment_s] == '.' && buf[segment_s + 1] == '.')
		{
			return 0;
		}
	}

	buf_size = 0;

	if (relative_uri->scheme.is_init)
	{
		buf_size += relative_uri->scheme.write_pos + 1;
	}
	if (relative_uri->authority.is_init)
	{
		buf_size += relative_uri->authority.write_pos + 2;
	}
	if (is_network_path || is_absolute_path)
	{
		buf_size += relative_uri->abs_path.write_pos;
	}
	else if (relative_uri->path.is_init)
	{
		buf_size += relative_uri->path.write_pos;
	}
	if (relative_uri->query.is_init)
	{
		buf_size += relative_uri->query.write_pos + 1;
	}
	if (relative_uri->fragment.is_init)
	{
		buf_size += relative_uri->fragment.write_pos + 1;
	}

	buf_size++;
	buf = (char*) malloc(buf_size);

	if (buf)
	{
		read_pos = 0;
		if (relative_uri->scheme.is_init)
		{
			buf_copy_size = relative_uri->scheme.write_pos;
			if (buf_copy_size > 0 && relative_uri->scheme.is_null_terminated)
			{
				buf_copy_size--;
			}
			memcpy(buf, relative_uri->scheme.buf, buf_copy_size);
			read_pos += buf_copy_size;
			memcpy(buf + read_pos, ":", 1);
			read_pos++;
		}
		if (relative_uri->authority.is_init)
		{
			buf_copy_size = relative_uri->authority.write_pos;
			if (buf_copy_size > 0 && relative_uri->authority.is_null_terminated)
			{
				buf_copy_size--;
			}
			memcpy(buf + read_pos, "//", 2);
			read_pos += 2;
			memcpy(buf + read_pos, relative_uri->authority.buf, buf_copy_size);
			read_pos += buf_copy_size;
		}
		if (is_network_path || is_absolute_path)
		{
			buf_copy_size = relative_uri->abs_path.write_pos;
			if (buf_copy_size > 0 && relative_uri->abs_path.is_null_terminated)
			{
				buf_copy_size--;
			}
			memcpy(buf + read_pos, relative_uri->abs_path.buf, buf_copy_size);
			read_pos += buf_copy_size;
		}
		else if (relative_uri->path.is_init)
		{
			buf_copy_size = relative_uri->path.write_pos;
			if (buf_copy_size > 0 && relative_uri->path.is_null_terminated)
			{
				buf_copy_size--;
			}
			memcpy(buf + read_pos, relative_uri->path.buf, buf_copy_size);
			read_pos += buf_copy_size;
		}
		if (relative_uri->query.is_init)
		{
			buf_copy_size = relative_uri->query.write_pos;
			if (buf_copy_size > 0 && relative_uri->query.is_null_terminated)
			{
				buf_copy_size--;
			}
			memcpy(buf + read_pos, "?", 1);
			read_pos++;
			memcpy(buf + read_pos, relative_uri->query.buf, buf_copy_size);
			read_pos += buf_copy_size;
		}
		if (relative_uri->fragment.is_init)
		{
			buf_copy_size = relative_uri->fragment.write_pos;
			if (buf_copy_size > 0 && relative_uri->fragment.is_null_terminated)
			{
				buf_copy_size--;
			}
			memcpy(buf + read_pos, "#", 1);
			read_pos++;
			memcpy(buf + read_pos, relative_uri->fragment.buf, buf_copy_size);
			read_pos += buf_copy_size;
		}
		buf[read_pos] = 0;
		relative_uri->uri.buf = buf;
		relative_uri->uri.buf_size = buf_size;
		relative_uri->uri.read_pos = read_pos;
		relative_uri->uri.flags = STRINGAA_IS_INIT |
			STRINGAA_IS_ALLOC | STRINGAA_IS_NULL_TERMINATED;
		relative_uri->uri.write_pos = relative_uri->uri.buf_size ? relative_uri->uri.buf_size - relative_uri->uri.is_null_terminated : 0;
	}
	else
	{
		return 0;
	}

	return 1;
}

/* Remove './' segments
 */
URIAA_INLINE int uriaa_remove_segment_point(stringaa *s)
{
	int is_set = 0;
	int i_s = 0;
	size_t segment_s = 0;
	char *buf = NULL;
	size_t buf_size = 0;
	size_t read_pos = 0;

	buf = s->buf;
	buf_size = s->buf_size;
	read_pos = s->read_pos;

	if (s)
	{
		is_set = 1;
		while(1)
		{
			while (i_s < buf_size)
			{
				if (buf[i_s] == '/')
				{
					if (i_s - segment_s == 1 && buf[i_s - 1] == '.')
					{
						memmove(buf + segment_s, buf + i_s + 1, buf_size - i_s);
						buf_size -= i_s + 1 - segment_s;
						i_s = segment_s;
						printf("buf: %s\n", buf);
						break;
					}
					segment_s = i_s + 1;
				}
				i_s++;
			}
			if (i_s >= buf_size)
			{
				break;
			}
		}
		if (segment_s + 1 == buf_size && buf[segment_s] == '.')
		{
			buf[segment_s] = 0;
		}
	}

	return is_set;
}

/* Remove '<segment>/../' segments
 */
URIAA_INLINE int uriaa_remove_segment_points(stringaa *s)
{
	int is_set = 0;
	int i_s = 0;
	size_t segment_s = 0;
	char *buf = NULL;
	size_t buf_size = 0;
	size_t read_pos = 0;

	buf = s->buf;
	buf_size = s->buf_size;
	read_pos = s->read_pos;

	i_s = 0;

	if (s)
	{
		is_set = 1;
		while (1)
		{
			segment_s = 0;

			while (i_s < buf_size)
			{
				if (buf[i_s] == '/')
				{
					/* segment is '../' */
					if (i_s - segment_s == 2 && buf[i_s - 1] == '.' && buf[i_s - 2] == '.')
					{
						segment_s = i_s + 1;
					}
					/* segment is '<segment>/..' */
					else if (i_s + 2 < buf_size && buf[i_s + 1] == '.' && buf[i_s + 2] == '.')
					{
						/* segment is '<segment>/../' */
						if (i_s + 3 < buf_size && buf[i_s + 3] == '/')
						{
							memmove(buf + segment_s, buf + i_s + 4, buf_size - i_s - 4 + 1);
							buf_size -= i_s + 4 - segment_s;
							i_s = 0;
							printf("buf: %s\n", buf);
							break;
						}
						/* segment is '<segment>/..$' */
						else if (i_s + 3 == buf_size)
						{
							memmove(buf + segment_s, buf + i_s + 3, buf_size - i_s - 3 + 1);
							buf_size -= i_s + 3 - segment_s;
							i_s = 0;
							printf("buf: %s\n", buf);
							break;
						}
					}
					/* segment is '<segment>/<segment>' */
					else
					{
						segment_s = i_s + 1;
					}
				}
				i_s++;
			}
			if (i_s >= buf_size)
			{
				break;
			}
		}

		s->buf_size = buf_size + 1;

		segment_s = 0;

		for (i_s = 0; i_s < buf_size; i_s++)
		{
			if (buf[i_s] == '/')
			{
				if (i_s - segment_s == 2 && buf[i_s - 1] == '.' && buf[i_s - 2] == '.')
				{
					return is_set;
				}
				else if (i_s - segment_s > 0)
				{
					break;
				}
				segment_s = i_s + 1;
			}
		}
		if (segment_s + 2 == buf_size && buf[segment_s] == '.' && buf[segment_s + 1] == '.')
		{
			return is_set;
		}
	}

	return is_set;
}

#endif	// URIAA_RFC2396_RESOLVE_H

