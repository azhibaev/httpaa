/* URI RFC 2396 Library.
 * Copyright (C) 2021  Askar Azhibaev
 *
 * File: uriaa_rfc2396_declarations.h
 * Description: Implementation of URI RFC 2396.
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: November 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef URIAA_RFC2396_DECLARATIONS_H
#define URIAA_RFC2396_DECLARATIONS_H

URIAA_INLINE int escaped(stringaa *uri);
URIAA_INLINE int URI_reference(uriaa *uri_reference);
URIAA_INLINE int absoluteURI(uriaa *uri_reference);
URIAA_INLINE int relativeURI(uriaa *uri_reference);
URIAA_INLINE int hier_part(uriaa *uri_reference);
URIAA_INLINE int opaque_part(uriaa *uri_reference);
URIAA_INLINE int net_path(uriaa *uri_reference);
URIAA_INLINE int abs_path(uriaa *uri_reference);
URIAA_INLINE int rel_path(uriaa *uri_reference);
URIAA_INLINE int rel_segment(uriaa *uri_reference);
URIAA_INLINE int scheme(uriaa *uri_reference);
URIAA_INLINE int authority(uriaa *uri_reference);
URIAA_INLINE int reg_name(uriaa *uri_reference);
URIAA_INLINE int server(uriaa *uri_reference);
URIAA_INLINE int userinfo(uriaa *uri_reference);
URIAA_INLINE int hostport(uriaa *uri_reference);
URIAA_INLINE int host(uriaa *uri_reference);
URIAA_INLINE int hostname(uriaa *uri_reference);
URIAA_INLINE int domainlabel(uriaa *uri_reference);
URIAA_INLINE int toplabel(uriaa *uri_reference);
URIAA_INLINE int IPv4address(uriaa *uri_reference);
URIAA_INLINE int port(uriaa *uri_reference);
URIAA_INLINE int path(uriaa *uri_reference);
URIAA_INLINE int path_segments(uriaa *uri_reference);
URIAA_INLINE int segment(uriaa *uri_reference);
URIAA_INLINE int param(uriaa *uri_reference);
URIAA_INLINE int query(uriaa *uri_reference);
URIAA_INLINE int fragment(uriaa *uri_reference);

#endif	/* URIAA_RFC2396_DECLARATIONS_H */

