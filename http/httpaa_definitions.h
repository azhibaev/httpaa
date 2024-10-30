/* HTTP/1.1 RFC 2616
 * Copyright (C) 2021  Askar Azhibaev
 *
 * File: httpaa_definitions.h
 * Description: Implementation of HTTP/1.1 RFC 2616
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: December 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef HTTPAA_DEFINITIONS_H
#define HTTPAA_DEFINITIONS_H

#define HTTP11_OCTET (c >= 0 && c <= 0xff)
#define HTTP11_CHAR (c >= 0 && c <= 127)
#define HTTP11_UPALPHA (c >= 'A' && c <= 'Z')
#define HTTP11_LOALPHA (c >= 'a' && c <= 'z')
#define HTTP11_ALPHA (UPALPHA || LOALPHA)
#define HTTP11_DIGIT (c >= '0' && c <= '9')
#define HTTP11_CTL ((c >= 0 && c <= 31) || c == 127)
#define HTTP11_CR (c == 13)
#define HTTP11_LF (c == 10)
#define HTTP11_SP (c == 32)
#define HTTP11_HT (c == 9)
#define HTTP11_DOUBLE_QUOTE_MARK (c == 34)
#define HTTP11_HEX ((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') || HTTP11_DIGIT)
#define HTTP11_SEPARATORS (c == '(' || c == ')' || c == '<' || c == '>' || \
		c == '@' || c == ',' || c == ';' || c == ':' || c == '\\' || \
		c == '"' || c == '/' || c == '[' || c == ']' || c == '?' || \
		c == '=' || c == '{' || c == '}' || HTTP11_SP || HTTP11_HT)

#endif	/* HTTPAA_DEFINITIONS_H */

