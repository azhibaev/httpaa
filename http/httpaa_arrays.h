/* HTTP/1.1 RFC 2616
 * Copyright (C) 2021  Askar Azhibaev
 *
 * File: httpaa_arrays.h
 * Description: Implementation of HTTP/1.1 RFC 2616
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: December 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef HTTPAA_ARRAYS_H
#define HTTPAA_ARRAYS_H

#define HTTP11_METHOD_ARRAY_SIZE 8
#define HTTP11_GENERAL_HEADER_ARRAY_SIZE 9
#define HTTP11_REQUEST_HEADER_ARRAY_SIZE 19
#define HTTP11_RESPONSE_HEADER_ARRAY_SIZE 9
#define HTTP11_ENTITY_HEADER_ARRAY_SIZE 10
#define HTTP11_GENERAL_HEADER_ARRAY_START 0
#define HTTP11_REQUEST_HEADER_ARRAY_START (HTTP11_GENERAL_HEADER_ARRAY_SIZE + HTTP11_ENTITY_HEADER_ARRAY_SIZE)
#define HTTP11_RESPONSE_HEADER_ARRAY_START (HTTP11_GENERAL_HEADER_ARRAY_SIZE + HTTP11_ENTITY_HEADER_ARRAY_SIZE)
#define HTTP11_ENTITY_HEADER_ARRAY_START HTTP11_GENERAL_HEADER_ARRAY_SIZE
#define HTTP11_STATUS_CODE_ARRAY_SIZE 40
#define HTTP11_HEADER_CACHE_REQUEST_DIRECTIVE_ARRAY_SIZE 7
#define HTTP11_HEADER_CACHE_RESPONSE_DIRECTIVE_ARRAY_SIZE 9
#define HTTP11_HEADER_PRAGMA_DIRECTIVE_ARRAY_SIZE 1
#define HTTP11_HEADER_TRANSFER_CODING_ARRAY_SIZE 5
#define HTTP11_DATE_WKDAY_ARRAY_SIZE 7
#define HTTP11_DATE_WEEKDAY_ARRAY_SIZE 7
#define HTTP11_DATE_MONTH_ARRAY_SIZE 12

enum http11_method_index
{
	http11_method_options,
	http11_method_get,
	http11_method_head,
	http11_method_post,
	http11_method_put,
	http11_method_delete,
	http11_method_trace,
	http11_method_connect
};

static const char *http11_method_array[HTTP11_METHOD_ARRAY_SIZE] = 
{
	"OPTIONS",
	"GET",
	"HEAD",
	"POST",
	"PUT",
	"DELETE",
	"TRACE",
	"CONNECT"
};

enum http11_general_header_index
{
	http11_general_header_cache_control = HTTP11_GENERAL_HEADER_ARRAY_START,
	http11_general_header_connection,
	http11_general_header_date,
	http11_general_header_pragma,
	http11_general_header_trailer,
	http11_general_header_transfer_encoding,
	http11_general_header_upgrade,
	http11_general_header_via,
	http11_general_header_warning
};

static const char *http11_general_header_array[HTTP11_GENERAL_HEADER_ARRAY_SIZE] = 
{
	"cache-control",
	"connection",
	"date",
	"pragma",
	"trailer",
	"transfer-encoding",
	"upgrade",
	"via",
	"warning"
};

static const char *http11_general_header_original_array[HTTP11_GENERAL_HEADER_ARRAY_SIZE] = 
{
	"Cache-Control",
	"Connection",
	"Date",
	"Pragma",
	"Trailer",
	"Transfer-Encoding",
	"Upgrade",
	"Via",
	"Warning"
};

enum http11_request_header_index
{
	http11_request_header_accept = HTTP11_REQUEST_HEADER_ARRAY_START,
	http11_request_header_accept_charset,
	http11_request_header_accept_encoding,
	http11_request_header_accept_language,
	http11_request_header_authorization,
	http11_request_header_expect,
	http11_request_header_from,
	http11_request_header_host,
	http11_request_header_if_match,
	http11_request_header_if_modified_since,
	http11_request_header_if_none_match,
	http11_request_header_if_range,
	http11_request_header_if_unmodified_since,
	http11_request_header_max_forwards,
	http11_request_header_proxy_authorization,
	http11_request_header_range,
	http11_request_header_referer,
	http11_request_header_te,
	http11_request_header_user_agent
};

static const char *http11_request_header_array[HTTP11_REQUEST_HEADER_ARRAY_SIZE] = 
{
	"accept",
	"accept-charset",
	"accept-encoding",
	"accept-language",
	"authorization",
	"expect",
	"from",
	"host",
	"if-match",
	"if-modified-since",
	"if-none-match",
	"if-range",
	"if-unmodified-since",
	"max-forwards",
	"proxy-authorization",
	"range",
	"referer",
	"te",
	"user-agent"
};

static const char *http11_request_header_original_array[HTTP11_REQUEST_HEADER_ARRAY_SIZE] = 
{
	"Accept",
	"Accept-Charset",
	"Accept-Encoding",
	"Accept-Language",
	"Authorization",
	"Expect",
	"From",
	"Host",
	"If-Match",
	"If-Modified-Since",
	"If-None-Match",
	"If-Range",
	"If-Unmodified-Since",
	"Max-Forwards",
	"Proxy-Authorization",
	"Range",
	"Referer",
	"Te",
	"User-Agent"
};

enum http11_entity_header_index
{
	http11_entity_header_allow = HTTP11_ENTITY_HEADER_ARRAY_START,
	http11_entity_header_content_encoding,
	http11_entity_header_content_language,
	http11_entity_header_content_length,
	http11_entity_header_content_location,
	http11_entity_header_content_md5,
	http11_entity_header_content_range,
	http11_entity_header_content_type,
	http11_entity_header_expires,
	http11_entity_header_last_modified
};

static const char *http11_entity_header_array[HTTP11_ENTITY_HEADER_ARRAY_SIZE] = 
{
	"allow",
	"content-encoding",
	"content-language",
	"content-length",
	"content-location",
	"content-md5",
	"content-range",
	"content-type",
	"expires",
	"last-modified"
};

static const char *http11_entity_header_original_array[HTTP11_ENTITY_HEADER_ARRAY_SIZE] = 
{
	"Allow",
	"Content-Encoding",
	"Content-Language",
	"Content-Length",
	"Content-Location",
	"Content-Md5",
	"Content-Range",
	"Content-Type",
	"Expires",
	"Last-Modified"
};

enum http11_response_header_index
{
	http11_response_header_accept_ranges = HTTP11_RESPONSE_HEADER_ARRAY_START,
	http11_response_header_age,
	http11_response_header_etag,
	http11_response_header_location,
	http11_response_header_proxy_authenticate,
	http11_response_header_retry_after,
	http11_response_header_server,
	http11_response_header_vary,
	http11_response_header_www_authenticate
};

static const char *http11_response_header_array[HTTP11_RESPONSE_HEADER_ARRAY_SIZE] = 
{
	"accept-ranges",
	"age",
	"etag",
	"location",
	"proxy-authenticate",
	"retry-after",
	"server",
	"vary",
	"www-authenticate"
};
/*
static const char *http11_response_header_original_array[HTTP11_RESPONSE_HEADER_ARRAY_SIZE] = 
{
	"Accept-Ranges",
	"Age",
	"Etag",
	"Location",
	"Proxy-Authenticate",
	"Retry-After",
	"Server",
	"Vary",
	"Www-Authenticate"
};
*/
enum http11_status_code_index
{
	http11_status_code_100_Continue = 100,
	http11_status_code_101_Switching_Protocols = 101,
	http11_status_code_200_OK = 200,
	http11_status_code_201_Created = 201,
	http11_status_code_202_Accepted = 202,
	http11_status_code_203_Non_Authoritative_Information = 203,
	http11_status_code_204_No_Content = 204,
	http11_status_code_205_Reset_Content = 205,
	http11_status_code_206_Partial_Content = 206,
	http11_status_code_300_Multiple_Choices = 300,
	http11_status_code_301_Moved_Permanently = 301,
	http11_status_code_302_Found = 302,
	http11_status_code_303_See_Other = 303,
	http11_status_code_304_Not_Modified = 304,
	http11_status_code_305_Use_Proxy = 305,
	http11_status_code_307_Temporary_Redirect = 307,
	http11_status_code_400_Bad_Request = 400,
	http11_status_code_401_Unauthorized = 401,
	http11_status_code_402_Payment_Required = 402,
	http11_status_code_403_Forbidden = 403,
	http11_status_code_404_Not_Found = 404,
	http11_status_code_405_Method_Not_Allowed = 405,
	http11_status_code_406_Not_Acceptable = 406,
	http11_status_code_407_Proxy_Authentication_Required = 407,
	http11_status_code_408_Request_Time_out = 408,
	http11_status_code_409_Conflict = 409,
	http11_status_code_410_Gone = 410,
	http11_status_code_411_Length_Required = 411,
	http11_status_code_412_Precondition_Failed = 412,
	http11_status_code_413_Request_Entity_Too_Large = 413,
	http11_status_code_414_Request_URI_Too_Large = 414,
	http11_status_code_415_Unsupported_Media_Type = 415,
	http11_status_code_416_Requested_range_not_satisfiable = 416,
	http11_status_code_417_Expectation_Failed = 417,
	http11_status_code_500_Internal_Server_Error = 500,
	http11_status_code_501_Not_Implemented = 501,
	http11_status_code_502_Bad_Gateway = 502,
	http11_status_code_503_Service_Unavailable = 503,
	http11_status_code_504_Gateway_Time_out = 504,
	http11_status_code_505_HTTP_Version_not_supported = 505
};

enum http11_status_code_key
{
	http11_status_code_key_100_Continue,
	http11_status_code_key_101_Switching_Protocols,
	http11_status_code_key_200_OK,
	http11_status_code_key_201_Created,
	http11_status_code_key_202_Accepted,
	http11_status_code_key_203_Non_Authoritative_Information,
	http11_status_code_key_204_No_Content,
	http11_status_code_key_205_Reset_Content,
	http11_status_code_key_206_Partial_Content,
	http11_status_code_key_300_Multiple_Choices,
	http11_status_code_key_301_Moved_Permanently,
	http11_status_code_key_302_Found,
	http11_status_code_key_303_See_Other,
	http11_status_code_key_304_Not_Modified,
	http11_status_code_key_305_Use_Proxy,
	http11_status_code_key_307_Temporary_Redirect,
	http11_status_code_key_400_Bad_Request,
	http11_status_code_key_401_Unauthorized,
	http11_status_code_key_402_Payment_Required,
	http11_status_code_key_403_Forbidden,
	http11_status_code_key_404_Not_Found,
	http11_status_code_key_405_Method_Not_Allowed,
	http11_status_code_key_406_Not_Acceptable,
	http11_status_code_key_407_Proxy_Authentication_Required,
	http11_status_code_key_408_Request_Time_out,
	http11_status_code_key_409_Conflict,
	http11_status_code_key_410_Gone,
	http11_status_code_key_411_Length_Required,
	http11_status_code_key_412_Precondition_Failed,
	http11_status_code_key_413_Request_Entity_Too_Large,
	http11_status_code_key_414_Request_URI_Too_Large,
	http11_status_code_key_415_Unsupported_Media_Type,
	http11_status_code_key_416_Requested_range_not_satisfiable,
	http11_status_code_key_417_Expectation_Failed,
	http11_status_code_key_500_Internal_Server_Error,
	http11_status_code_key_501_Not_Implemented,
	http11_status_code_key_502_Bad_Gateway,
	http11_status_code_key_503_Service_Unavailable,
	http11_status_code_key_504_Gateway_Time_out,
	http11_status_code_key_505_HTTP_Version_not_supported
};

static const int http11_status_code_value[HTTP11_STATUS_CODE_ARRAY_SIZE] = 
{
	100, /* 100 Continue */
	101, /* 101 Switching Protocols */
	200, /* 200 OK */
	201, /* 201 Created */
	202, /* 202 Accepted */
	203, /* 203 Non-Authoritative Information */
	204, /* 204 No Content */
	205, /* 205 Reset Content */
	206, /* 206 Partial Content */
	300, /* 300 Multiple Choices */
	301, /* 301 Moved Permanently */
	302, /* 302 Found */
	303, /* 303 See Other */
	304, /* 304 Not Modified */
	305, /* 305 Use Proxy */
	307, /* 307 Temporary Redirect */
	400, /* 400 Bad Request */
	401, /* 401 Unauthorized */
	402, /* 402 Payment Required */
	403, /* 403 Forbidden */
	404, /* 404 Not Found */
	405, /* 405 Method Not Allowed */
	406, /* 406 Not Acceptable */
	407, /* 407 Proxy Authentication Required */
	408, /* 408 Request Time-out */
	409, /* 409 Conflict */
	410, /* 410 Gone */
	411, /* 411 Length Required */
	412, /* 412 Precondition Failed */
	413, /* 413 Request Entity Too Large */
	414, /* 414 Request-URI Too Large */
	415, /* 415 Unsupported Media Type */
	416, /* 416 Requested range not satisfiable */
	417, /* 417 Expectation Failed */
	500, /* 500 Internal Server Error */
	501, /* 501 Not Implemented */
	502, /* 502 Bad Gateway */
	503, /* 503 Service Unavailable */
	504, /* 504 Gateway Time-out */
	505  /* 505 HTTP Version not supported */
};

static const char *http11_status_code_array[HTTP11_STATUS_CODE_ARRAY_SIZE * 2] = 
{
	"100", "Continue",
	"101", "Switching Protocols",
	"200", "OK",
	"201", "Created",
	"202", "Accepted",
	"203", "Non-Authoritative Information",
	"204", "No Content",
	"205", "Reset Content",
	"206", "Partial Content",
	"300", "Multiple Choices",
	"301", "Moved Permanently",
	"302", "Found",
	"303", "See Other",
	"304", "Not Modified",
	"305", "Use Proxy",
	"307", "Temporary Redirect",
	"400", "Bad Request",
	"401", "Unauthorized",
	"402", "Payment Required",
	"403", "Forbidden",
	"404", "Not Found",
	"405", "Method Not Allowed",
	"406", "Not Acceptable",
	"407", "Proxy Authentication Required",
	"408", "Request Time-out",
	"409", "Conflict",
	"410", "Gone",
	"411", "Length Required",
	"412", "Precondition Failed",
	"413", "Request Entity Too Large",
	"414", "Request-URI Too Large",
	"415", "Unsupported Media Type",
	"416", "Requested range not satisfiable",
	"417", "Expectation Failed",
	"500", "Internal Server Error",
	"501", "Not Implemented",
	"502", "Bad Gateway",
	"503", "Service Unavailable",
	"504", "Gateway Time-out",
	"505", "HTTP Version not supported"
};

enum http11_header_cache_request_directive_index
{
	http11_header_cache_request_directive_no_cache,
	http11_header_cache_request_directive_no_store,
	http11_header_cache_request_directive_max_age,
	http11_header_cache_request_directive_max_stale,
	http11_header_cache_request_directive_min_fresh,
	http11_header_cache_request_directive_no_transform,
	http11_header_cache_request_directive_only_if_cached
};

static const char *http11_header_cache_request_directive_array[HTTP11_HEADER_CACHE_REQUEST_DIRECTIVE_ARRAY_SIZE] = 
{
	"no-cache",
	"no-store",
	"max-age",
	"max-stale",
	"min-fresh",
	"no-transform",
	"only-if-cached"
};

enum http11_header_cache_response_directive_index
{
	http11_header_cache_response_directive_public,
	http11_header_cache_response_directive_private,
	http11_header_cache_response_directive_no_cache,
	http11_header_cache_response_directive_no_store,
	http11_header_cache_response_directive_no_transform,
	http11_header_cache_response_directive_must_revalidate,
	http11_header_cache_response_directive_proxy_revalidate,
	http11_header_cache_response_directive_max_age,
	http11_header_cache_response_directive_s_maxage
};

static const char *http11_header_cache_response_directive_array[HTTP11_HEADER_CACHE_RESPONSE_DIRECTIVE_ARRAY_SIZE] = 
{
	"public",
	"private",
	"no-cache",
	"no-store",
	"no-transform",
	"must-revalidate",
	"proxy-revalidate",
	"max-age",
	"s-maxage"
};

enum http11_header_pragma_directive_index
{
	http11_header_pragma_directive_no_cache
};

static const char *http11_header_pragma_directive_array[HTTP11_HEADER_PRAGMA_DIRECTIVE_ARRAY_SIZE] = 
{
	"no-cache"
};

enum http11_header_transfer_coding_index
{
	http11_header_transfer_coding_chunked,
	http11_header_transfer_coding_identity,
	http11_header_transfer_coding_gzip,
	http11_header_transfer_coding_compress,
	http11_header_transfer_coding_deflate
};

static const char *http11_header_transfer_coding_array[HTTP11_HEADER_TRANSFER_CODING_ARRAY_SIZE] = 
{
	"chunked",
	"identity",
	"gzip",
	"compress",
	"deflate"
};

enum http11_date_wkday_index
{
	http11_date_wkday_mon,
	http11_date_wkday_tue,
	http11_date_wkday_wed,
	http11_date_wkday_thu,
	http11_date_wkday_fri,
	http11_date_wkday_sat,
	http11_date_wkday_sun
};

static const char *http11_date_wkday_array[HTTP11_DATE_WKDAY_ARRAY_SIZE] =
{
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat",
	"Sun"
};

enum http11_date_weekday_index
{
	http11_date_weekday_monday,
	http11_date_weekday_tuesday,
	http11_date_weekday_wednesday,
	http11_date_weekday_thursday,
	http11_date_weekday_friday,
	http11_date_weekday_saturday,
	http11_date_weekday_sunday
};

static const char *http11_date_weekday_array[HTTP11_DATE_WEEKDAY_ARRAY_SIZE] =
{
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday",
	"Sunday"
};

enum http11_date_month_index
{
	http11_date_month_jan,
	http11_date_month_feb,
	http11_date_month_mar,
	http11_date_month_apr,
	http11_date_month_may,
	http11_date_month_jun,
	http11_date_month_jul,
	http11_date_month_aug,
	http11_date_month_sep,
	http11_date_month_oct,
	http11_date_month_nov,
	http11_date_month_dec
};

static const char *http11_date_month_array[HTTP11_DATE_MONTH_ARRAY_SIZE] =
{
	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec"
};

#endif	/* HTTPAA_ARRAYS_H */

