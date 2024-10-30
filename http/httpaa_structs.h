/* HTTP/1.1 RFC 2616
 * Copyright (C) 2021  Askar Azhibaev
 *
 * File: httpaa_structs.h
 * Description: Implementation of HTTP/1.1 RFC 2616
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: December 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef HTTPAA_STRUCTS_H
#define HTTPAA_STRUCTS_H

struct http11_http_version_struct
{
	int major_version;
	int minor_version;
};

struct http11_request_line_struct
{
	enum http11_method_index method;
	uriaa request_uri;
	struct http11_http_version_struct http_version;
};

struct http11_general_header_struct
{
	stringaa Cache_Control;
	stringaa Connection;
	stringaa Date;
	stringaa Pragma;
	stringaa Trailer;
	stringaa Transfer_Encoding;
	stringaa Upgrade;
	stringaa Via;
	stringaa Warning;
};

struct http11_request_header_struct
{
	stringaa Accept;
	stringaa Accept_Charset;
	stringaa Accept_Encoding;
	stringaa Accept_Language;
	stringaa Authorization;
	stringaa Expect;
	stringaa From;
	stringaa Host;
	stringaa If_Match;
	stringaa If_Modified_Since;
	stringaa If_None_Match;
	stringaa If_Range;
	stringaa If_Unmodified_Since;
	stringaa Max_Forwards;
	stringaa Proxy_Authorization;
	stringaa Range;
	stringaa Referer;
	stringaa TE;
	stringaa User_Agent;
};

struct http11_entity_header_struct
{
	stringaa Allow;
	stringaa Content_Encoding;
	stringaa Content_Language;
	stringaa Content_Length;
	stringaa Content_Location;
	stringaa Content_MD5;
	stringaa Content_Range;
	stringaa Content_Type;
	stringaa Expires;
	stringaa Last_Modified;
};

struct http11_response_header_struct
{
	stringaa Accept_Ranges;
	stringaa Age;
	stringaa ETag;
	stringaa Location;
	stringaa Proxy_Authenticate;
	stringaa Retry_After;
	stringaa Server;
	stringaa Vary;
	stringaa WWW_Authenticate;
};

struct http11_message_body_struct
{
	stringaa entity_body;
};

struct http11_status_line_struct
{
	struct http11_http_version_struct http_version;
	stringaa reason_phrase;
};

struct http11_request_struct
{
	struct http11_request_line_struct request_line;
	stringaa header[HTTP11_GENERAL_HEADER_ARRAY_SIZE + 
		HTTP11_REQUEST_HEADER_ARRAY_SIZE + 
		HTTP11_ENTITY_HEADER_ARRAY_SIZE];
	struct http11_message_body_struct message_body;
};

struct http11_response_struct
{
	struct http11_status_line_struct status_line;
	stringaa header[HTTP11_GENERAL_HEADER_ARRAY_SIZE + 
		HTTP11_RESPONSE_HEADER_ARRAY_SIZE + 
		HTTP11_ENTITY_HEADER_ARRAY_SIZE];
	struct http11_message_body_struct message_body;
};

struct http11_http_struct
{
	struct http11_request_struct request;
	struct http11_response_struct response;
	stringaa request_file_name;
	stringaa response_file_name;
};

typedef struct http11_http_struct httpaa;

#endif	/* HTTPAA_STRUCTS_H */

