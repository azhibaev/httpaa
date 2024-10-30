/* URI RFC 2396 Library.
 * Copyright (C) 2021  Askar Azhibaev
 *
 * File: uriaa_utils.h
 * Description: Implementation of URI RFC 2396.
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: November 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef URIAA_UTILS_H
#define URIAA_UTILS_H

URIAA_INLINE int uriaa_check_url(uriaa *uri_reference)
{
	int is_check_url = 0;

	if (uri_reference)
	{
		if (uri_reference->uri.buf && uri_reference->uri.write_pos > 0 &&
				uri_reference->uri.is_init)
		{
			is_check_url = URI_reference(uri_reference);
		}
	}

	return is_check_url;
}

URIAA_INLINE int uriaa_escape_char(char ca[2], char c)
{
	int is_escape = 1;
	int i = 0;

	for (i = 0 ; i < 2; i++)
	{
		ca[i] = ((unsigned char)c & 0xf) | 0x30;
		if (ca[i] > 0x39)
		{
			ca[i] += 0x7;
		}
		c >>= 4;
	}

	return is_escape;
}

/* Return unescaped char in ca[2]
 */
URIAA_INLINE int uriaa_unescape_char(char ca[3])
{
	int is_unescape = 0;
	char c = 0;
	int i = 0;

	ca[2] = 0;

	for (i = 0; i < 2; i++)
	{
		c = ca[i];
		if (c >= 'a' && c <= 'f')
		{
			c -= 0x20;
		}
		if (c >= 'A' && c <= 'F')
		{
			c -= 0x7;
		}
		if (c >= 0x30 && c <= 0x3f)
		{
			ca[2] |= (c & 0xf) << (i << 2);
			printf("ca[2] = %x\n", ca[2]);
		}
		else
		{
			return 0;
		}
	}

	is_unescape = 1;

	return is_unescape;
}

/* Unescape all or unreserved chars
 * flags = 0 or URIAA_ESCAPE_UNRESERVED
 */
URIAA_INLINE int uriaa_unescape(stringaa *uri, stringaa *uri_escaped, int flags)
{
	int is_unescape = 0;
	char c = 0;
	char ca[3] = {0};

	stringaa_malloc(uri, uri_escaped->write_pos);
	stringaa_begin_write_pos(uri);
	uri->is_init = 1;
	stringaa_begin_read_pos(uri_escaped);

	while (1)
	{
		c = stringaa_get_char(uri_escaped);

		if (c == '%')
		{
			if (!stringaa_next_read_pos(uri_escaped))
			{
				break;
			}
			ca[1] = stringaa_get_char(uri_escaped);
			if (!stringaa_next_read_pos(uri_escaped))
			{
				break;
			}
			ca[0] = stringaa_get_char(uri_escaped);

			is_unescape = uriaa_unescape_char(ca);
			if (is_unescape)
			{
				if (flags & URIAA_ESCAPE_UNRESERVED)
				{
					c = ca[2];
					if (UNRESERVED_CHARS)
					{
						stringaa_set_char(uri, c);
						stringaa_next_write_pos(uri);
					}
					else
					{
						stringaa_set_char(uri, '%');
						stringaa_next_write_pos(uri);
						stringaa_set_char(uri, ca[1]);
						stringaa_next_write_pos(uri);
						stringaa_set_char(uri, ca[0]);
						stringaa_next_write_pos(uri);
					}
				}
				else
				{
					stringaa_set_char(uri, ca[0]);
					stringaa_next_write_pos(uri);
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			stringaa_set_char(uri, c);
			stringaa_next_write_pos(uri);
		}

		if (!stringaa_next_read_pos(uri_escaped))
		{
			is_unescape = 1;
			break;
		}
	}

	return is_unescape;
}

/* Escape
 * flags = 0 or URIAA_ESCAPE_RESERVED
 */
URIAA_INLINE int uriaa_escape(stringaa *uri_escaped, stringaa *uri, int flags)
{
	int is_escape = 0;
	char c = 0;
	char ca[2] = {0};
	size_t size = uri->write_pos;

	printf("size = %zu\n", size);
	stringaa_malloc(uri_escaped, size * 3);
	printf("size3 = %zu\n", uri_escaped->buf_size);
	stringaa_begin_write_pos(uri_escaped);
	uri_escaped->is_init = 1;
	stringaa_begin_read_pos(uri);
	
	while (1)
	{
		c = stringaa_get_char(uri);
		printf("c = %c\n", c);

		if (CONTROL_CHARS || SPACE_CHARS || DELIMS_CHARS || UNWISE_CHARS)
		{
			is_escape = uriaa_escape_char(ca, c);
			printf("c[1] = %c; c[0] = %c\n", ca[1], ca[0]);
		}
		else if ((flags & URIAA_ESCAPE_RESERVED) && (RESERVED_CHARS))
		{
			is_escape = uriaa_escape_char(ca, c);
			printf("c[1] = %c; c[0] = %c\n", ca[1], ca[0]);
		}

		if (is_escape)
		{
			stringaa_set_char(uri_escaped, '%');
			stringaa_next_write_pos(uri_escaped);
			stringaa_set_char(uri_escaped, ca[1]);
			stringaa_next_write_pos(uri_escaped);
			stringaa_set_char(uri_escaped, ca[0]);
			stringaa_next_write_pos(uri_escaped);
			is_escape = 0;
		}
		else
		{
			stringaa_set_char(uri_escaped, c);
			stringaa_next_write_pos(uri_escaped);
		}

		if (!stringaa_next_read_pos(uri))
		{
			is_escape = 1;
			break;
		}
		printf("read = %zu (%x); write = %zu (%x)\n", uri->read_pos, uri->buf[uri->read_pos], uri->write_pos, uri->buf[uri->write_pos]);
	}

	return is_escape;
}

URIAA_INLINE int uriaa_print(uriaa *uri)
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

#endif	// URIAA_UTILS_H

