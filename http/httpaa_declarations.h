/* HTTP/1.1 RFC 2616
 * Copyright (C) 2021  Askar Azhibaev
 *
 * File: httpaa_declarations.h
 * Description: Implementation of HTTP/1.1 RFC 2616
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: December 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef HTTPAA_HEADERS_H
#define HTTPAA_HEADERS_H

HTTPAA_INLINE int http11_crlf(stringaa *request);
HTTPAA_INLINE int http11_find_crlf(stringaa *request);
HTTPAA_INLINE int http11_lws(stringaa *request);
HTTPAA_INLINE int http11_text(stringaa *request);
HTTPAA_INLINE int http11_token(stringaa *request);
HTTPAA_INLINE int http11_comment(stringaa *request);
HTTPAA_INLINE int http11_ctext(stringaa *request);
HTTPAA_INLINE int http11_quoted_string(stringaa *request);
HTTPAA_INLINE int http11_qdtext(stringaa *request);
HTTPAA_INLINE int http11_quoted_pair(stringaa *request);
HTTPAA_INLINE int http11_http_version(httpaa *http,
		stringaa *request);
HTTPAA_INLINE int http11_request(httpaa *http,
		stringaa *request);
HTTPAA_INLINE int http11_request_line(httpaa *http,
		stringaa *request);
HTTPAA_INLINE int http11_method(httpaa *http,
		stringaa *request);
HTTPAA_INLINE int http11_general_header(httpaa *http,
		stringaa *request);
HTTPAA_INLINE int http11_request_header(httpaa *http,
		stringaa *request);
HTTPAA_INLINE int http11_response_header(httpaa *http,
		stringaa *request);
HTTPAA_INLINE int http11_entity_header(httpaa *http,
		stringaa *request);
HTTPAA_INLINE int http11_message_body(stringaa *request);
HTTPAA_INLINE int http11_request_uri(httpaa *http,
		stringaa *request);
HTTPAA_INLINE int http11_message_header(httpaa *http,
		stringaa *request);
HTTPAA_INLINE int http11_field_name(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_field_value(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_field_content(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_cache_control(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_cache_directive(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_cache_request_directive(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_cache_response_directive(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_connection(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_date(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_date(stringaa *request);
HTTPAA_INLINE int http11_rfc1123_date(stringaa *request);
HTTPAA_INLINE int http11_rfc850_date(stringaa *request);
HTTPAA_INLINE int http11_asctime_date(stringaa *request);
HTTPAA_INLINE int http11_date1(stringaa *request);
HTTPAA_INLINE int http11_date2(stringaa *request);
HTTPAA_INLINE int http11_date3(stringaa *request);
HTTPAA_INLINE int http11_time(stringaa *request);
HTTPAA_INLINE int http11_wkday(stringaa *request);
HTTPAA_INLINE int http11_weekday(stringaa *request);
HTTPAA_INLINE int http11_month(stringaa *request);
HTTPAA_INLINE int http11_delta_seconds(stringaa *request);
HTTPAA_INLINE int http11_header_pragma(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_pragma_directive(stringaa *request);
HTTPAA_INLINE int http11_header_extension_pragma(stringaa *request);
HTTPAA_INLINE int http11_header_trailer(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_transfer_encoding(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_transfer_coding(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_transfer_extension(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_parameter(stringaa *request);
HTTPAA_INLINE int http11_header_upgrade(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_product(stringaa *request);
HTTPAA_INLINE int http11_header_via(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_received_protocol(stringaa *request);
HTTPAA_INLINE int http11_received_by(stringaa *request);
HTTPAA_INLINE int http11_host(stringaa *request);
HTTPAA_INLINE int http11_port(stringaa *request);
HTTPAA_INLINE int http11_header_warning(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_warning_value(stringaa *request);
HTTPAA_INLINE int http11_warning_code(stringaa *request);
HTTPAA_INLINE int http11_warning_agent(stringaa *request);
HTTPAA_INLINE int http11_header_accept(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_media_range(stringaa *request);
HTTPAA_INLINE int http11_accept_params(stringaa *request);
HTTPAA_INLINE int http11_accept_extension(stringaa *request);
HTTPAA_INLINE int http11_qvalue(stringaa *request);
HTTPAA_INLINE int http11_header_accept_charset(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_charset(stringaa *request);
HTTPAA_INLINE int http11_header_accept_encoding(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_codings(stringaa *request);
HTTPAA_INLINE int http11_content_coding(stringaa *request);
HTTPAA_INLINE int http11_header_accept_language(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_language_range(stringaa *request);
HTTPAA_INLINE int http11_header_authorization(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_credentials(stringaa *request);
HTTPAA_INLINE int http11_header_expect(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_expectation(stringaa *request);
HTTPAA_INLINE int http11_expectation_extension(stringaa *request);
HTTPAA_INLINE int http11_expect_params(stringaa *request);
HTTPAA_INLINE int http11_header_from(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_mailbox(stringaa *request);
HTTPAA_INLINE int http11_header_host(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_if_match(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_entity_tag(stringaa *request);
HTTPAA_INLINE int http11_header_if_modified_since(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_if_none_match(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_if_range(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_if_unmodified_since(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_max_forwards(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_proxy_authorization(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_range(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_bytes_unit(stringaa *request);
HTTPAA_INLINE int http11_byte_range_set(stringaa *request);
HTTPAA_INLINE int http11_byte_range_spec(stringaa *request);
HTTPAA_INLINE int http11_suffix_byte_range_spec(stringaa *request);
HTTPAA_INLINE int http11_header_referer(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_te(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_t_codings(httpaa *http,
		stringaa *request);
HTTPAA_INLINE int http11_header_user_agent(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_accept_ranges(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_range_unit(stringaa *request);
HTTPAA_INLINE int http11_header_age(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_etag(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_location(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_proxy_authenticate(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_challenge(stringaa *request);
HTTPAA_INLINE int http11_header_retry_after(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_server(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_vary(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_www_authenticate(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_allow(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_content_encoding(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_content_language(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_language_tag(stringaa *request);
HTTPAA_INLINE int http11_header_content_length(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_content_location(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_content_md5(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_content_range(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_content_type(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_media_type(stringaa *request);
HTTPAA_INLINE int http11_header_expires(httpaa *http,
        stringaa *request);
HTTPAA_INLINE int http11_header_last_modified(httpaa *http,
        stringaa *request);

#endif	/* HTTPAA_HEADERS_H */

