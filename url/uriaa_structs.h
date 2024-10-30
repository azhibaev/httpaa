/* URI RFC 2396 Library.
 * Copyright (C) 2021  Askar Azhibaev
 *
 * File: uriaa_structs.h
 * Description: Implementation of URI RFC 2396.
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: November 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef URIAA_STRUCTS_H
#define URIAA_STRUCTS_H

struct uri_struct
{
	stringaa uri;
	stringaa scheme;
	stringaa authority;
	stringaa userinfo;
	stringaa host;
	stringaa port;
	stringaa path;
	stringaa net_path;
	stringaa abs_path;
	stringaa rel_path;
	stringaa opaque_part;
	stringaa query;
	stringaa fragment;
};

typedef struct uri_struct uriaa;

#endif	// URIAA_STRUCTS_H

