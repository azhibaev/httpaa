/* String Library. Mathematics functions.
 * Copyright (C) 2021  Askar Azhibaev
 *
 * File: stringaa_math.h
 * Description: Mathematics functions for string library
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: December 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef STRINGAA_MATH_H
#define STRINGAA_MATH_H

#include "stringaa.h"

/* Multiply c1 and c2
 * result in
 * s[0] is high digit
 * s[1] is low digit
 * s[2] is term
 */
STRINGAA_INLINE char* stringaa_mul_char(char s[3], char c1, char c2)
{
	int i = 0;

	i = c1 * c2 + s[0] + s[2];

	s[0] = i / 10;
	s[1] = i % 10;
	
	return s;
}

/* Multiply s1 and s2
 * result in s
 */
STRINGAA_INLINE int stringaa_mul(stringaa *s, stringaa *s1, stringaa *s2)
{
	char c = 0;
	char c2[3] = {0};
	size_t s_pos = 0;
	size_t buf_size = 0;
	size_t s1_buf_size = 0;
	size_t s2_buf_size = 0;

	if (s1->is_int == 0)
	{
		stringaa_chars_to_int(s1);
	}
	if (s2->is_int == 0)
	{
		stringaa_chars_to_int(s2);
	}
	stringaa_begin_pos(s1);
	stringaa_begin_pos(s2);

	while (stringaa_get_char(s1) == 0)
	{
		if (!stringaa_next_pos(s1))
		{
			break;
		}
	}
	while (stringaa_get_char(s2) == 0)
	{
		if (!stringaa_next_pos(s2))
		{
			break;
		}
	}
	
	s1_buf_size = s1->buf_size - s1->buf_pos;
	s2_buf_size = s2->buf_size - s2->buf_pos;
	buf_size = s1_buf_size + s2_buf_size + 1;
	stringaa_set_size(buf_size, s);
	memset(s->buf, 0, s->buf_size);

	stringaa_end_pos(s);
	stringaa_end_pos(s1);
	stringaa_end_pos(s2);

	while (1)
	{
		s_pos = s->buf_pos;
		c = stringaa_get_char(s2);
		while (1)
		{
			c2[2] = stringaa_get_char(s);
			stringaa_mul_char(c2, stringaa_get_char(s1), c);
			stringaa_set_char(c2[1], s);
			if (!stringaa_prev_pos(s))
			{
				break;
			}
			if (!stringaa_prev_pos(s1))
			{
				if (c2[0] == 0)
				{
					break;
				}
			}
		}
		if (!stringaa_prev_pos(s2))
		{
			break;
		}
		stringaa_end_pos(s1);
		s->buf_pos = s_pos;
		stringaa_prev_pos(s);
	}

	s->is_init = 1;

	return 1;
}

/* Convert char to int
 * example: '0' -> '\0'
 */
STRINGAA_INLINE char stringaa_char_to_int(char c)
{
	c &= 0xf;

	return c;
}

/* Convert int to char
 * example: '\0' -> '0'
 */
STRINGAA_INLINE char stringaa_int_to_char(char c)
{
	c |= 0x30;

	return c;
}

/* Convert array of chars to array of int */
STRINGAA_INLINE int stringaa_chars_to_int(stringaa *s)
{
	char c = 0;

	if (s->is_alloc == 0)
	{
		if (!stringaa_malloc_copy(s))
		{
			return 0;
		}
	}

	stringaa_begin_pos(s);

	while (1)
	{
		c = stringaa_get_char(s);
		c &= 0xf;
		stringaa_set_char(c, s);
		if (!stringaa_next_pos(s))
		{
			break;
		}
	}

	return 1;
}

/* Convert array of int to array of chars */
STRINGAA_INLINE int stringaa_int_to_chars(stringaa *s)
{
	char c = 0;

	if (s->is_alloc == 0)
	{
		if (!stringaa_malloc_copy(s))
		{
			return 0;
		}
	}

	stringaa_begin_pos(s);

	while (1)
	{
		c = stringaa_get_char(s);
		c |= 0x30;
		stringaa_set_char(c, s);
		if (!stringaa_next_pos(s))
		{
			break;
		}
	}

	return 1;
}

/* Add c1 and c2
 * result in
 * s[0] is high digit
 * s[1] is low digit
 */
STRINGAA_INLINE char* stringaa_add_char(char s[2], char c1, char c2)
{
	int i = 0;

	i = c1 + c2 + s[0];

	s[0] = i / 10;
	s[1] = i % 10;
	
	return s;
}

/* Add s1 and s2
 * result in s
 */
STRINGAA_INLINE int stringaa_add(stringaa *s, stringaa *s1, stringaa *s2)
{
	char c2[2] = {0};

	if (s1->is_int == 0)
	{
		stringaa_chars_to_int(s1);
	}
	if (s2->is_int == 0)
	{
		stringaa_chars_to_int(s2);
	}

	stringaa_set_size((s1->buf_size > s2->buf_size ? s1->buf_size : s2->buf_size) + 2, s);
	s->buf[s->buf_size - 1] = 0;

	stringaa_end_pos(s);
	stringaa_end_pos(s1);
	stringaa_end_pos(s2);

	while (1)
	{
		stringaa_add_char(c2, stringaa_get_char(s1), stringaa_get_char(s2));
		stringaa_set_char(c2[1], s);
		if (!stringaa_prev_pos(s))
		{
			break;
		}
		stringaa_prev_pos(s1);
		stringaa_prev_pos(s2);
	}

	s->is_init = 1;

	return 1;
}

/* Add s2 to s1
 */
STRINGAA_INLINE int stringaa_add_to(stringaa *s1, stringaa *s2)
{
	char c2[2] = {0};
	size_t buf_size = 0;
	size_t s1_buf_size = 0;
	size_t s2_buf_size = 0;

	if (s1->is_int == 0)
	{
		stringaa_chars_to_int(s1);
	}
	if (s2->is_int == 0)
	{
		stringaa_chars_to_int(s2);
	}
	stringaa_begin_pos(s1);
	stringaa_begin_pos(s2);

	while (stringaa_get_char(s1) == 0)
	{
		if (!stringaa_next_pos(s1))
		{
			break;
		}
	}
	while (stringaa_get_char(s2) == 0)
	{
		if (!stringaa_next_pos(s2))
		{
			break;
		}
	}
	
	s1_buf_size = s1->buf_size - s1->buf_pos;
	s2_buf_size = s2->buf_size - s2->buf_pos;
	buf_size = s1_buf_size > s2_buf_size ? s1_buf_size : s2_buf_size;
	if (s1->buf_size < buf_size + 1)
	{
		stringaa_set_size((s1->buf_size > s2->buf_size ? s1->buf_size : s2->buf_size) + 2, s1);
		s1->buf[s1->buf_size - 1] = 0;
	}

	stringaa_end_pos(s1);
	stringaa_end_pos(s2);

	while (1)
	{
		stringaa_add_char(c2, stringaa_get_char(s1), stringaa_get_char(s2));
		stringaa_set_char(c2[1], s1);
		if (!stringaa_prev_pos(s1))
		{
			break;
		}
		stringaa_prev_pos(s2);
	}

	s1->is_init = 1;

	return 1;
}

/* Sub c2 and c1
 * result in
 * s[0] is high digit
 * s[1] is low digit
 */
STRINGAA_INLINE char* stringaa_sub_char(char s[2], char c1, char c2)
{
	char c = 0;

	c = c1 - c2 - s[0];
	if (c < 0)
	{
		c += 10;
		s[0] = 1;
		s[1] = c + 10;
	}
	else
	{
		s[0] = 0;
		s[1] = c;
	}
	
	return s;
}

/* Sub s2 from s1
 */
STRINGAA_INLINE int stringaa_sub(stringaa *s1, stringaa *s2)
{
	char c2[2] = {0};

	if (s1->is_int == 0)
	{
		stringaa_chars_to_int(s1);
	}
	if (s2->is_int == 0)
	{
		stringaa_chars_to_int(s2);
	}
	stringaa_begin_pos(s1);
	stringaa_begin_pos(s2);
	if (stringaa_get_char(s1) || stringaa_get_char(s2))
	{
		stringaa_set_size((s1->buf_size > s2->buf_size ? s1->buf_size : s2->buf_size) + 2, s1);
		s1->buf[s1->buf_size - 1] = 0;
	}

	stringaa_end_pos(s1);
	stringaa_end_pos(s2);

	while (1)
	{
		stringaa_add_char(c2, stringaa_get_char(s1), stringaa_get_char(s2));
		stringaa_set_char(c2[1], s1);
		if (!stringaa_prev_pos(s1))
		{
			break;
		}
		stringaa_prev_pos(s2);
	}

	s1->is_init = 1;

	return 1;
}

/* Return -1 if (int)s1 < (int)s2
 * return 0 if (int)s1 == (int)s2
 * return 1 if (int)s1 > (int)s2
 */
STRINGAA_INLINE int stringaa_compare(stringaa *s1, stringaa *s2)
{
	int i = 0;

	stringaa_begin_pos(s1);
	stringaa_begin_pos(s2);

	while (stringaa_get_char(s1) == 0)
	{
		if (!stringaa_next_pos(s1))
		{
			break;
		}
	}
	while (stringaa_get_char(s2) == 0)
	{
		if (!stringaa_next_pos(s2))
		{
			break;
		}
	}
	if (s1->buf_size - s1->buf_pos < s2->buf_size - s2->buf_pos)
	{
		return -1;
	}
	else if (s1->buf_size - s1->buf_pos > s2->buf_size - s2->buf_pos)
	{
		return 1;
	}
	else
	{
		while (1)
		{
			i = stringaa_get_char(s1) - stringaa_get_char(s2);
			if (i < 0)
			{
				return -1;
			}
			else if (i > 0)
			{
				return 1;
			}
			if (!stringaa_next_pos(s1) || !stringaa_next_pos(s2))
			{
				break;
			}
		}
	}

	return 0;
}

#endif	/* STRINGAA_MATH_H */

