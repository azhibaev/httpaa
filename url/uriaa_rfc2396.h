/* URI RFC 2396 Library.
 * Copyright (C) 2021  Askar Azhibaev
 *
 * File: uriaa_rfc2396.h
 * Description: Implementation of URI RFC 2396.
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: November 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef URIAA_RFC2396_H
#define URIAA_RFC2396_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stringaa.h"
#include "uriaa_structs.h"

#define URIAA_INLINE static inline

#include "uriaa_rfc2396_declarations.h"

/* Chars */
#define DIGIT_CHARS (c >= '0' && c <= '9')
#define LOWALPHA_CHARS (c >= 'a' && c <= 'z')
#define UPALPHA_CHARS (c >= 'A' && c <= 'Z')
#define ALPHA_CHARS (LOWALPHA_CHARS || UPALPHA_CHARS)
#define ALPHANUM_CHARS (ALPHA_CHARS || DIGIT_CHARS)
#define HEX_CHARS (DIGIT_CHARS || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
#define ESCAPED_CHARS (escaped(uri))
#define MARK_CHARS (c == '-' || c == '_' || c == '.' || c == '!' || c == '~' \
	|| c == '*' || c == '\'' || c == '(' || c == ')')
#define UNRESERVED_CHARS (ALPHANUM_CHARS || MARK_CHARS)
#define RESERVED_CHARS (c == ';' || c == '/' || c == '?' || c == ':' \
	|| c == '@' || c == '&' || c == '=' || c == '+' || c == '$' || c == ',')
#define URIC_CHARS (RESERVED_CHARS || UNRESERVED_CHARS || ESCAPED_CHARS)
#define PCHAR_CHARS (UNRESERVED_CHARS || ESCAPED_CHARS || c == ':' \
	|| c == '@' || c == '&' || c == '=' || c == '+' || c == '$' || c == ',')
#define USERINFO_CHARS (UNRESERVED_CHARS || ESCAPED_CHARS || c == ';' \
	|| c == ':' || c == '&' || c == '=' || c == '+' || c == '$' || c == ',')
#define REG_NAME_CHARS (UNRESERVED_CHARS || ESCAPED_CHARS || c == '$' \
	|| c == ',' || c == ';' || c == ':' || c == '@' || c == '&' || c == '=' \
	|| c == '+')
#define SCHEME_CHARS (ALPHA_CHARS || DIGIT_CHARS || c == '+' || c == '-' \
	|| c == '.')
#define REL_SEGMENT_CHARS (UNRESERVED_CHARS || ESCAPED_CHARS || c == ';' \
	|| c == '@' || c == '&' || c == '=' || c == '+' || c == '$' || c == ',')
#define URIC_NO_SLASH_CHARS (UNRESERVED_CHARS || ESCAPED_CHARS || c == ';' \
	|| c == '?' || c == ':' || c == '@' || c == '&' || c == '=' || c == '+' \
	|| c == '$' || c == ',')

/* Escape chars */
#define CONTROL_CHARS ((c >= 0 && c <= 0x1f) || c == 0x7f)
#define SPACE_CHARS (c == 0x20)
#define DELIMS_CHARS (c == '<' || c == '>' || c == '#' || c == '%' || c == '"')
#define UNWISE_CHARS (c == '{' || c == '}' || c == '|' || c == '\\' \
	|| c == '^'	|| c == '[' || c == ']' || c == '`')

/* Escape flags */
#define URIAA_ESCAPE_RESERVED	(1 << 0)
#define URIAA_ESCAPE_UNRESERVED	(1 << 1)
#define URIAA_ESCAPE_EXCLUDED	(1 << 2)

URIAA_INLINE int escaped(stringaa *uri)
{
	char c = 0;
	size_t pos = uri->read_pos;
	//const char *buf = uri->buf;

	c = stringaa_get_char(uri);

	if (c == '%')
	{
		if (stringaa_next_read_pos(uri))
		{
			c = stringaa_get_char(uri);
			if (HEX_CHARS)
			{
				if (stringaa_next_read_pos(uri))
				{
					c = stringaa_get_char(uri);
					if (HEX_CHARS)
					{
						stringaa_next_read_pos(uri);
						return 1;
					}
				}
			}
			uri->read_pos = pos;
		}
	}

	return 0;
}

URIAA_INLINE int URI_reference(uriaa *uri_reference)
{
	int is_URI_reference = 0;
	int is_absoluteURI = 0;
	int is_relativeURI = 0;
	int is_fragment = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	if (absoluteURI(uri_reference))
	{
		is_absoluteURI = 1;
	}
	else if (relativeURI(uri_reference))
	{
		is_relativeURI = 1;
	}

	if (stringaa_is_char(uri, '#'))
	{
		if (fragment(uri_reference))
		{
			is_fragment = 1;
		}
	}

	is_URI_reference = is_absoluteURI || is_relativeURI || is_fragment;

	if (is_URI_reference)
	{
	}
	else
	{
		uri->read_pos = pos;
	}

	return is_URI_reference;
}

URIAA_INLINE int absoluteURI(uriaa *uri_reference)
{
	int is_absoluteURI = 0;
	int is_scheme = 0;
	int is_hier_part = 0;
	int is_opaque_part = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	if (scheme(uri_reference))
	{
		if (stringaa_is_char(uri, ':'))
		{
			is_scheme = 1;
			if (hier_part(uri_reference))
			{
				is_hier_part = 1;
			}
			else if (opaque_part(uri_reference))
			{
				is_opaque_part = 1;
			}
		}
		else
		{
			uri_reference->scheme.buf_size = 0;
			uri_reference->scheme.write_pos = 0;
			uri_reference->scheme.is_init = 0;
		}
	}

	is_absoluteURI = is_scheme && (is_hier_part | is_opaque_part);

	if (is_absoluteURI)
	{
	}
	else
	{
		uri->read_pos = pos;
	}

	return is_absoluteURI;
}

URIAA_INLINE int relativeURI(uriaa *uri_reference)
{
	int is_relativeURI = 0;
	int is_net_path = 0;
	int is_abs_path = 0;
	int is_rel_path = 0;
	int is_query = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	if (net_path(uri_reference))
	{
		is_net_path = 1;
	}
	else if (abs_path(uri_reference))
	{
		is_abs_path = 1;
	}
	else if (rel_path(uri_reference))
	{
		is_rel_path = 1;
	}

	if (stringaa_is_char(uri, '?'))
	{
		if (query(uri_reference))
		{
			is_query = 1;
		}
	}

	is_relativeURI = is_net_path || is_abs_path || is_rel_path;

	if (is_relativeURI)
	{
	}
	else
	{
		uri->read_pos = pos;
	}

	return is_relativeURI;
}

URIAA_INLINE int hier_part(uriaa *uri_reference)
{
	int is_hier_part = 0;
	int is_net_path = 0;
	int is_abs_path = 0;
	int is_query = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	if (net_path(uri_reference))
	{
		is_net_path = 1;
	}
	else if (abs_path(uri_reference))
	{
		is_abs_path = 1;
	}

	if (stringaa_is_char(uri, '?'))
	{
		if (query(uri_reference))
		{
			is_query = 1;
		}
	}

	is_hier_part = is_net_path || is_abs_path;

	if (is_hier_part)
	{
	}
	else
	{
		uri->read_pos = pos;
	}

	return is_hier_part;
}

URIAA_INLINE int opaque_part(uriaa *uri_reference)
{
	int is_opaque_part = 0;
	char c = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	c = stringaa_get_char(uri);

	if (URIC_NO_SLASH_CHARS)
	{
		is_opaque_part = 1;
		if (stringaa_next_read_pos(uri))
		{
			c = stringaa_get_char(uri);
			while (URIC_CHARS)
			{
				if (stringaa_next_read_pos(uri))
				{
					c = stringaa_get_char(uri);
				}
				else
				{
					break;
				}
			}
		}
	}

	if (is_opaque_part)
	{
		uri_reference->opaque_part.buf = uri->buf + pos;
		uri_reference->opaque_part.buf_size = uri->read_pos - pos;
		uri_reference->opaque_part.write_pos = uri_reference->opaque_part.buf_size ? uri_reference->opaque_part.buf_size - uri_reference->opaque_part.is_null_terminated : 0;
		uri_reference->opaque_part.is_init = 1;
	}
	else
	{
	}

	return is_opaque_part;
}

URIAA_INLINE int net_path(uriaa *uri_reference)
{
	int is_net_path = 0;
	int is_authority = 0;
	int is_abs_path = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	if (stringaa_is_char(uri, '/'))
	{
		if (stringaa_is_char(uri, '/'))
		{
			if (authority(uri_reference))
			{
				is_authority = 1;
				if (abs_path(uri_reference))
				{
					is_abs_path = 1;
				}
			}
		}
	}

	is_net_path = is_authority;

	if (is_net_path)
	{
		uri_reference->net_path.buf = uri->buf + pos;
		uri_reference->net_path.buf_size = uri->read_pos - pos;
		uri_reference->net_path.write_pos = uri_reference->net_path.buf_size ? uri_reference->net_path.buf_size - uri_reference->net_path.is_null_terminated : 0;
		uri_reference->net_path.is_init = 1;
	}
	else
	{
		uri->read_pos = pos;
	}

	return is_net_path;
}

URIAA_INLINE int abs_path(uriaa *uri_reference)
{
	int is_abs_path = 0;
	int is_path_segments = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	if (stringaa_is_char(uri, '/'))
	{
		if (path_segments(uri_reference))
		{
			is_path_segments = 1;
		}
	}

	is_abs_path = is_path_segments;

	if (is_abs_path)
	{
		uri_reference->abs_path.buf = uri->buf + pos;
		uri_reference->abs_path.buf_size = uri->read_pos - pos;
		uri_reference->abs_path.write_pos = uri_reference->abs_path.buf_size ? uri_reference->abs_path.buf_size - uri_reference->abs_path.is_null_terminated : 0;
		uri_reference->abs_path.is_init = 1;
	}
	else
	{
		uri->read_pos = pos;
	}

	return is_abs_path;
}

URIAA_INLINE int rel_path(uriaa *uri_reference)
{
	int is_rel_path = 0;
	int is_rel_segment = 0;
	int is_abs_path = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	if (rel_segment(uri_reference))
	{
		is_rel_segment = 1;
		if (abs_path(uri_reference))
		{
			is_abs_path = 1;
		}
	}

	is_rel_path = is_rel_segment;

	if (is_rel_path)
	{
		uri_reference->rel_path.buf = uri->buf + pos;
		uri_reference->rel_path.buf_size = uri->read_pos - pos;
		uri_reference->rel_path.write_pos = uri_reference->rel_path.buf_size ? uri_reference->rel_path.buf_size - uri_reference->rel_path.is_null_terminated : 0;
		uri_reference->rel_path.is_init = 1;
	}
	else
	{
		uri->read_pos = pos;
	}

	return is_rel_path;
}

URIAA_INLINE int rel_segment(uriaa *uri_reference)
{
	int is_rel_segment = 0;
	char c = 0;
	stringaa *uri = &uri_reference->uri;

	c = stringaa_get_char(uri);
	if (REL_SEGMENT_CHARS)
	{
		is_rel_segment = 1;
		if (stringaa_next_read_pos(uri))
		{
			c = stringaa_get_char(uri);
			while (REL_SEGMENT_CHARS)
			{
				if (stringaa_next_read_pos(uri))
				{
					c = stringaa_get_char(uri);
				}
				else
				{
					break;
				}
			}
		}
	}

	return is_rel_segment;
}

URIAA_INLINE int scheme(uriaa *uri_reference)
{
	int is_scheme = 0;
	char c = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	c = stringaa_get_char(uri);
	if (ALPHA_CHARS)
	{
		is_scheme = 1;
		if (stringaa_next_read_pos(uri))
		{
			c = stringaa_get_char(uri);
			while (ALPHA_CHARS || DIGIT_CHARS || c == '+' || c == '-' || c == '.')
			{
				if (stringaa_next_read_pos(uri))
				{
					c = stringaa_get_char(uri);
				}
				else
				{
					break;
				}
			}
		}
	}

	if (is_scheme)
	{
		uri_reference->scheme.buf = uri->buf + pos;
		uri_reference->scheme.buf_size = uri->read_pos - pos;
		uri_reference->scheme.write_pos = uri_reference->scheme.buf_size ? uri_reference->scheme.buf_size - uri_reference->scheme.is_null_terminated : 0;
		uri_reference->scheme.is_init = 1;
	}
	else
	{
	}

	return is_scheme;
}

URIAA_INLINE int authority(uriaa *uri_reference)
{
	int is_authority = 0;
	int is_server = 0;
	int is_reg_name = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	if (server(uri_reference))
	{
		is_server = 1;
	}
	else if (reg_name(uri_reference))
	{
		is_reg_name = 1;
	}

	is_authority = is_server || is_reg_name;

	if (is_authority)
	{
		uri_reference->authority.buf = uri->buf + pos;
		uri_reference->authority.buf_size = uri->read_pos - pos;
		uri_reference->authority.write_pos = uri_reference->authority.buf_size ? uri_reference->authority.buf_size - uri_reference->authority.is_null_terminated : 0;
		uri_reference->authority.is_init = 1;
	}
	else
	{
		uri->read_pos = pos;
	}

	return is_authority;
}

URIAA_INLINE int reg_name(uriaa *uri_reference)
{
	int is_reg_name = 0;
	char c = 0;
	stringaa *uri = &uri_reference->uri;

	c = stringaa_get_char(uri);
	if (REG_NAME_CHARS)
	{
		is_reg_name = 1;
		if (stringaa_next_read_pos(uri))
		{
			c = stringaa_get_char(uri);
			while (REG_NAME_CHARS)
			{
				if (stringaa_next_read_pos(uri))
				{
					c = stringaa_get_char(uri);
				}
				else
				{
					break;
				}
			}
		}
	}

	return is_reg_name;
}

URIAA_INLINE int server(uriaa *uri_reference)
{
	int is_server = 0;
	int is_userinfo = 0;
	int is_hostport = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	if (userinfo(uri_reference))
	{
		if (stringaa_is_char(uri, '@'))
		{
			is_userinfo = 1;
		}
	}

	if (is_userinfo)
	{
		uri_reference->userinfo.buf = uri->buf + pos;
		uri_reference->userinfo.buf_size = uri->read_pos - pos - 1;
		uri_reference->userinfo.write_pos = uri_reference->userinfo.buf_size ? uri_reference->userinfo.buf_size - uri_reference->userinfo.is_null_terminated : 0;
		uri_reference->userinfo.is_init = 1;
	}
	else
	{
		uri->read_pos = pos;
	}

	if (hostport(uri_reference))
	{
		is_hostport = 1;
	}
	else
	{
		uri->read_pos = pos;
		uri_reference->userinfo.buf_size = 0;
		uri_reference->userinfo.write_pos = 0;
		uri_reference->userinfo.is_init = 0;
	}

	is_server = 1;

	return is_server;
}

URIAA_INLINE int userinfo(uriaa *uri_reference)
{
	int is_userinfo = 0;
	char c = 0;
	stringaa *uri = &uri_reference->uri;

	c = stringaa_get_char(uri);
	while (USERINFO_CHARS)
	{
		if (stringaa_next_read_pos(uri))
		{
			c = stringaa_get_char(uri);
		}
		else
		{
			break;
		}
	}

	is_userinfo = 1;

	return is_userinfo;
}

URIAA_INLINE int hostport(uriaa *uri_reference)
{
	int is_hostport = 0;
	int is_host = 0;
	int is_port = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	if (host(uri_reference))
	{
		is_host = 1;
		if (stringaa_is_char(uri, ':'))
		{
			if (port(uri_reference))
			{
				is_port = 1;
			}
		}
	}

	is_hostport = is_host;

	if (is_hostport)
	{
	}
	else
	{
		uri->read_pos = pos;
	}

	return is_hostport;
}

URIAA_INLINE int host(uriaa *uri_reference)
{
	int is_host = 0;
	int is_hostname = 0;
	int is_IPv4address = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	if (hostname(uri_reference))
	{
		is_hostname = 1;
	}
	else if (IPv4address(uri_reference))
	{
		is_IPv4address = 1;
	}

	is_host = is_hostname || is_IPv4address;

	if (is_host)
	{
		uri_reference->host.buf = uri->buf + pos;
		uri_reference->host.buf_size = uri->read_pos - pos;
		uri_reference->host.write_pos = uri_reference->host.buf_size ? uri_reference->host.buf_size - uri_reference->host.is_null_terminated : 0;
		uri_reference->host.is_init = 1;
	}
	else
	{
		uri->read_pos = pos;
	}

	return is_host;
}

URIAA_INLINE int hostname(uriaa *uri_reference)
{
	int is_hostname = 0;
	int is_domainlabel = 0;
	int is_toplabel = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;
	size_t pos1 = pos;
	size_t label_pos = pos;

	while (1)
	{
		pos1 = uri->read_pos;
		if (domainlabel(uri_reference))
		{
			label_pos = pos1;
			if (stringaa_is_char(uri, '.'))
			{
				is_domainlabel = 1;
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

	uri->read_pos = label_pos;

	if (toplabel(uri_reference))
	{
		is_toplabel = 1;
		if (stringaa_is_char(uri, '.'))
		{
		}
	}

	is_hostname = is_toplabel;

	if (is_hostname)
	{
	}
	else
	{
		uri->read_pos = pos;
	}

	return is_hostname;
}

URIAA_INLINE int domainlabel(uriaa *uri_reference)
{
	int is_domainlabel = 0;
	char c = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	c = stringaa_get_char(uri);

	if (ALPHANUM_CHARS)
	{
		is_domainlabel = 1;
		while (1)
		{
			if (stringaa_next_read_pos(uri))
			{
				c = stringaa_get_char(uri);
			}
			else
			{
				break;
			}
			if (ALPHANUM_CHARS || c == '-')
			{
			}
			else
			{
				if (uri->buf[uri->read_pos - 1] == '-')
				{
					is_domainlabel = 0;
				}
				break;
			}
		}
	}

	if (is_domainlabel)
	{
	}
	else
	{
		uri->read_pos = pos;
	}

	return is_domainlabel;
}

URIAA_INLINE int toplabel(uriaa *uri_reference)
{
	int is_toplabel = 0;
	char c = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	c = stringaa_get_char(uri);

	if (ALPHA_CHARS)
	{
		is_toplabel = 1;
		while (1)
		{
			if (stringaa_next_read_pos(uri))
			{
				c = stringaa_get_char(uri);
			}
			else
			{
				break;
			}
			if (ALPHANUM_CHARS || c == '-')
			{
			}
			else
			{
				if (uri->buf[uri->read_pos - 1] == '-')
				{
					is_toplabel = 0;
				}
				break;
			}
		}
	}

	if (is_toplabel)
	{
	}
	else
	{
		uri->read_pos = pos;
	}

	return is_toplabel;
}

URIAA_INLINE int IPv4address(uriaa *uri_reference)
{
	int is_IPv4address = 0;
	int is_digit = 0;
	char c = 0;
	int i = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	for (i = 0; i < 4; i++)
	{
		c = stringaa_get_char(uri);
		if (DIGIT_CHARS)
		{
			if (stringaa_next_read_pos(uri))
			{
				c = stringaa_get_char(uri);
			}
			else
			{
				break;
			}
			while (DIGIT_CHARS)
			{
				if (stringaa_next_read_pos(uri))
				{
					c = stringaa_get_char(uri);
				}
				else
				{
					break;
				}
			}
			if (i < 3)
			{
				if (stringaa_is_char(uri, '.'))
				{
				}
				else
				{
					break;
				}
			}
			is_digit++;
		}
	}

	is_IPv4address = (is_digit == 4);

	if (is_IPv4address)
	{
	}
	else
	{
		uri->read_pos = pos;
	}

	return is_IPv4address;
}

URIAA_INLINE int port(uriaa *uri_reference)
{
	int is_port = 0;
	char c = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	c = stringaa_get_char(uri);

	while (DIGIT_CHARS)
	{
		is_port = 1;
		if (stringaa_next_read_pos(uri))
		{
			c = stringaa_get_char(uri);
		}
		else
		{
			break;
		}
	}

	if (is_port)
	{
		uri_reference->port.buf = uri->buf + pos;
		uri_reference->port.buf_size = uri->read_pos - pos;
		uri_reference->port.write_pos = uri_reference->port.buf_size ? uri_reference->port.buf_size - uri_reference->port.is_null_terminated : 0;
		uri_reference->port.is_init = 1;
	}
	else
	{
		uri->read_pos = pos;
	}

	return is_port;
}

URIAA_INLINE int path(uriaa *uri_reference)
{
	int is_path = 0;
	int is_abs_path = 0;
	int is_opaque_path = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	if (abs_path(uri_reference))
	{
		is_abs_path = 1;
	}
	else if (opaque_part(uri_reference))
	{
		is_opaque_path = 1;
	}

	is_path = is_abs_path || is_opaque_path;

	if (is_path)
	{
		uri_reference->path.buf = uri->buf + pos;
		uri_reference->path.buf_size = uri->read_pos - pos;
		uri_reference->path.write_pos = uri_reference->path.buf_size ? uri_reference->path.buf_size - uri_reference->path.is_null_terminated : 0;
		uri_reference->path.is_init = 1;
	}
	else
	{
		uri->read_pos = pos;
	}

	return is_path;
}

URIAA_INLINE int path_segments(uriaa *uri_reference)
{
	int is_path_segments = 0;
	int is_segment = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	if (segment(uri_reference))
	{
		is_segment = 1;
		while (1)
		{
			if (stringaa_is_char(uri, '/'))
			{
				if (segment(uri_reference))
				{
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
	}

	is_path_segments = is_segment;

	if (is_path_segments)
	{
	}
	else
	{
		uri->read_pos = pos;
	}

	return is_path_segments;
}

URIAA_INLINE int segment(uriaa *uri_reference)
{
	int is_segment = 1;
	char c = 0;
	stringaa *uri = &uri_reference->uri;

	c = stringaa_get_char(uri);

	while (PCHAR_CHARS)
	{
		if (stringaa_next_read_pos(uri))
		{
			c = stringaa_get_char(uri);
		}
		else
		{
			break;
		}
	}

	while (1)
	{
		if (stringaa_is_char(uri, ';'))
		{
			if (param(uri_reference))
			{
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

	return is_segment;
}

URIAA_INLINE int param(uriaa *uri_reference)
{
	int is_param = 1;
	char c = 0;
	stringaa *uri = &uri_reference->uri;

	c = stringaa_get_char(uri);

	while (PCHAR_CHARS)
	{
		if (stringaa_next_read_pos(uri))
		{
			c = stringaa_get_char(uri);
		}
		else
		{
			break;
		}
	}

	return is_param;
}

URIAA_INLINE int query(uriaa *uri_reference)
{
	int is_query = 1;
	char c = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	c = stringaa_get_char(uri);

	while (URIC_CHARS)
	{
		if (stringaa_next_read_pos(uri))
		{
			c = stringaa_get_char(uri);
		}
		else
		{
			break;
		}
	}

	if (is_query)
	{
		uri_reference->query.buf = uri->buf + pos;
		uri_reference->query.buf_size = uri->read_pos - pos;
		uri_reference->query.write_pos = uri_reference->query.buf_size ? uri_reference->query.buf_size - uri_reference->query.is_null_terminated : 0;
		uri_reference->query.is_init = 1;
	}

	return is_query;
}

URIAA_INLINE int fragment(uriaa *uri_reference)
{
	int is_fragment = 1;
	char c = 0;
	stringaa *uri = &uri_reference->uri;
	size_t pos = uri->read_pos;

	c = stringaa_get_char(uri);

	while (URIC_CHARS)
	{
		if (stringaa_next_read_pos(uri))
		{
			c = stringaa_get_char(uri);
		}
		else
		{
			break;
		}
	}

	if (is_fragment)
	{
		uri_reference->fragment.buf = uri->buf + pos;
		uri_reference->fragment.buf_size = uri->read_pos - pos;
		uri_reference->fragment.write_pos = uri_reference->fragment.buf_size ? uri_reference->fragment.buf_size - uri_reference->fragment.is_null_terminated : 0;
		uri_reference->fragment.is_init = 1;
	}

	return is_fragment;
}

#endif	// URIAA_RFC2396_H

