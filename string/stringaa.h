/* String Library.
 * Copyright (C) 2021  Askar Azhibaev
 *
 * File: stringaa.h
 * Description: String library
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: December 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef STRINGAA_H
#define STRINGAA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STRINGAA_INLINE static inline
#define STRINGAA_FLAG(flag) (1 << (flag))

#define STRINGAA_BEGIN_POS	0	/* Start of buffer */
#define STRINGAA_READ_POS 	-1	/* Read position */
#define STRINGAA_WRITE_POS	-2	/* Write position */
#define STRINGAA_END_POS	-3	/* End of buffer */

struct string_struct
{
	/* Pointer to char array
	 * can be allocated or link to other array
	 * can be null-terminated or not
	 */
	char *buf;
	/* Full size of array
	 * if string is null-terminated then last zero byte is uncluded, 
	 * that is strlen(buf) + 1;
	 * otherwise, if null-terminated flag is reset, 
	 * then it is length of string,
	 * for example length of string 'abcdef' is 6;
	 * if buf_size == 0 then string is empty
	 */
	size_t buf_size;
	/* Position in array
	 * first char in 0 position
	 * read_pos = 0; read_pos < buf_size;
	 */
	size_t read_pos;
	/* Write position in array
	 * first char in 0 position
	 * write_pos = 0; write_pos <= buf_size;
	 */
	size_t write_pos;
	/* Size of available data
	 * (not using)
	 */
	size_t buf_data_size;
	/* Pointer to original buf
	 */
	char *buf_original;
	/* Pointer to previous string
	 * if is_link = 1
	 * original buf <-- link1 <-- link2
	 */
	struct string_struct *prev_link;
	/* Pointer to next string
	 * if is_link = 1
	 * original buf --> link1 --> link2
	 * of remove original string, link1 became new original
	 * if remove link1, link2 points to original
	 * original buf --> link2
	 */
	struct string_struct *next_link;
	union
	{
		/* stringaa_flags */
		unsigned int flags;
		struct
		{
			/* Set if string is initialized and defined 
			 * reset if string is undefined
			 */
			unsigned int is_init : 1;
			/* Set if array is allocated from memory */
			unsigned int is_alloc : 1;
			/* Set if string is null-terminated */
			unsigned int is_null_terminated : 1;
			/* Set if buf points to other array */
			unsigned int is_link : 1;
			/* Set if array of binary values */
			unsigned int is_binary : 1;
		};
	};
};

enum stringaa_flags_index
{
	stringaa_flags_is_init,
	stringaa_flags_is_alloc,
	stringaa_flags_is_null_terminated,
	stringaa_flags_is_link,
	stringaa_flags_is_binary
};

enum stringaa_flags
{
	STRINGAA_IS_INIT = STRINGAA_FLAG(stringaa_flags_is_init),
	STRINGAA_IS_ALLOC = STRINGAA_FLAG(stringaa_flags_is_alloc),
	STRINGAA_IS_NULL_TERMINATED = STRINGAA_FLAG(stringaa_flags_is_null_terminated),
	STRINGAA_IS_LINK = STRINGAA_FLAG(stringaa_flags_is_link),
	STRINGAA_IS_BINARY = STRINGAA_FLAG(stringaa_flags_is_binary)
};

typedef struct string_struct stringaa;

/* Print string
 * to f stream
 */
STRINGAA_INLINE int stringaa_fprint(FILE *f, stringaa *s)
{
	int i = 0;
	const char *buf = NULL;
	size_t buf_size = 0;

	if (s == NULL)
	{
		return -1;
	}
	if (s->is_init == 0)
	{
		return -1;
	}

	buf = s->buf;
	buf_size = s->write_pos + 1;
	if (buf_size > 0 && s->is_null_terminated)
	{
		buf_size--;
	}

	if (s->is_binary)
	{
		for (i = 0; i < buf_size; i++)
		{
			fputc(buf[i] | 0x30, f);
		}
	}
	else
	{
		for (i = 0; i < buf_size; i++)
		{
			fputc(buf[i], f);
		}
	}

	return 0;
}


/* Print string
 * and '\n'
 * to f stream
 */
STRINGAA_INLINE int stringaa_fprint_line(FILE *f, stringaa *s)
{
	int is_print = 0;

	is_print = stringaa_fprint(f, s);
	fprintf(f, "\n");

	return is_print;
}

/* Print string */
STRINGAA_INLINE int stringaa_print(stringaa *s)
{
	int is_print = 0;

	is_print = stringaa_fprint(stdout, s);

	return is_print;
}

/* Print string
 * and '\n'
 */
STRINGAA_INLINE int stringaa_print_line(stringaa *s)
{
	int is_print = 0;

	is_print = stringaa_print(s);
	printf("\n");

	return is_print;
}

/* Set size of buf,
 * buf_size include null 
 */
STRINGAA_INLINE int stringaa_malloc(stringaa *s, const size_t buf_size)
{
	int is_set = 0;
	char *buf = NULL;

	if (buf_size > 0)
	{
		if ((s->is_alloc && s->buf_size < buf_size) || s->is_alloc == 0)
		{
			buf = (char*) malloc(buf_size);
			if (buf)
			{
				memset(buf, 0, buf_size);
				if (s->is_alloc)
				{
					free(s->buf);
				}
				s->buf = buf;
				s->buf_size = buf_size;
				s->read_pos = 0;
				s->write_pos = 0;
				s->flags = STRINGAA_IS_ALLOC |
					STRINGAA_IS_NULL_TERMINATED;
				is_set = 1;
			}
		}
	}

	return is_set;
}

/* Set new buf and copy
 * can not convert to null-terminated */
STRINGAA_INLINE int stringaa_realloc(stringaa *s, size_t buf_size)
{
	char *buf = NULL;

		if (buf_size == 0 || buf_size < s->buf_size)
		{
			return 0;
		}
		if (s->is_null_terminated == 0)
		{
			buf_size++;
		}
		buf = (char*) malloc(buf_size);
		if (buf == NULL)
		{
			return 0;
		}
		if (s->is_init)
		{
			if (s->write_pos >= s->buf_size)
			{
				if (s->buf_size > 0)
				{
					s->write_pos = s->buf_size - 1;
				}
				else
				{
					s->write_pos = 0;
				}
			}
			memcpy(buf, s->buf, s->write_pos);
			if (s->is_null_terminated == 1)
			{
				buf[s->write_pos] = 0;
			}
		}
		else
		{
			s->write_pos = 0;
		}
		s->buf = buf;
		s->buf_size = buf_size;
		s->read_pos = 0;
		s->flags |= STRINGAA_IS_ALLOC;

	return 1;
}

/* Set pos to next char
 * 0 <= read_pos < write_pos <= buf_size
 * Hello!0
 * ^     ^
 * r     w ; r = 0 ; w = 6 ; size = 7 ; is_null_terminated = 1
 * Hello!
 * ^     ^
 * r     w ; r = 0 ; w = 6 ; size = 6 ; is_null_terminated = 0
 * 0 <= r < w
 * r = w : end of read
 * 0 <= w + is_null_terminated < size
 * w = size - is_null_terminated : end of write
 * Always r < w
 * w <= size + 1 for is_null_terminated = 0
 */
STRINGAA_INLINE int stringaa_next_read_pos(stringaa *s)
{
	/* 0 <= read < write */
	if (s->read_pos + 1 < s->write_pos)
	{
		s->read_pos++;
		return 1;
	}
	else
	{
		/* End of read */
		s->read_pos = s->write_pos;
	}

	return 0;
}

/* Set pos to prev char */
STRINGAA_INLINE int stringaa_prev_read_pos(stringaa *s)
{
	/* 0 <= read < write */
	if (s->read_pos > 0 && s->read_pos <= s->write_pos)
	{
		s->read_pos--;
		return 1;
	}
	else
	{
		/* End of read */
		s->read_pos = s->write_pos;
	}

	return 0;
}

/* Set pos to first char */
STRINGAA_INLINE int stringaa_begin_read_pos(stringaa *s)
{
	/* Begin of read */
	s->read_pos = 0;

	return 1;
}

/* Set pos to last char */
STRINGAA_INLINE int stringaa_end_read_pos(stringaa *s)
{
	s->read_pos = s->write_pos;
	/* 0 <= read < write */
	if (s->read_pos > 0)
	{
		s->read_pos--;
	}

	return 1;
}

/* Set pos to next char */
STRINGAA_INLINE int stringaa_next_write_pos(stringaa *s)
{
	/* 0 <= write + is_null_terminated < size */
	if (s->write_pos + s->is_null_terminated + 1 < s->buf_size)
	{
		s->write_pos++;
		return 1;
	}
	else
	{
		s->write_pos = s->buf_size ? s->buf_size - s->is_null_terminated : 0;
	}

	return 0;
}

/* Set pos to prev char */
STRINGAA_INLINE int stringaa_prev_write_pos(stringaa *s)
{
	/* 0 <= write + is_null_terminated < size */
	if (s->write_pos > 0 && s->write_pos + s->is_null_terminated <= s->buf_size)
	{
		s->write_pos--;
		return 1;
	}
	else
	{
		s->write_pos = s->buf_size ? s->buf_size - s->is_null_terminated : 0;
	}

	return 0;
}

/* Set pos to first char */
STRINGAA_INLINE int stringaa_begin_write_pos(stringaa *s)
{
	s->write_pos = 0;

	return 1;
}

/* Set pos to last char */
STRINGAA_INLINE int stringaa_end_write_pos(stringaa *s)
{
	s->write_pos = s->buf_size ? s->buf_size - s->is_null_terminated : 0;
	if (s->write_pos > 0)
	{
		s->write_pos--;
	}

	return 1;
}

/* Return char at current pos */
STRINGAA_INLINE char stringaa_get_char(stringaa *s)
{
	char c = 0;

	/* 0 <= read < write */
	if (s->read_pos < s->write_pos)
	{
		c = s->buf[s->read_pos];
	}

	return c;
}

/* Set char at current pos */
STRINGAA_INLINE int stringaa_set_char(stringaa *s, const char c)
{
	int is_set = 0;

	if (s->write_pos + s->is_null_terminated < s->buf_size)
	{
		if (s->is_alloc == 0)
		{
			stringaa_realloc(s, 0);
		}

		s->buf[s->write_pos] = c;
		is_set = 1;
	}

	return is_set;
}

/* Return 1 if c = char at current pos,
 * and set next pos
 */
STRINGAA_INLINE int stringaa_is_char(stringaa *s, const char c)
{
	if (c == stringaa_get_char(s))
	{
		stringaa_next_read_pos(s);
		return 1;
	}

	return 0;
}

/* Return 1 if c = char at current pos,
 * and set next pos
 * case ignored
 */
STRINGAA_INLINE int stringaa_is_chari(stringaa *request, const char c)
{
	if (_toupper(c) == _toupper(request->buf[request->read_pos]))
	{
		request->read_pos++;
		return 1;
	}

	return 0;
}

/* Set pointer to buf,
 * set size
 */
STRINGAA_INLINE int stringaa_set_chars_size(stringaa *s, const char *buf, const size_t buf_size)
{
	int is_set = 0;

	if (s && buf && buf_size)
	{
		if (s->is_alloc)
		{
			free(s->buf);
		}

		s->buf = (char*)buf;
		s->buf_size = buf_size;
		s->read_pos = 0;
		s->flags = STRINGAA_IS_INIT;
		s->write_pos = s->buf_size ? s->buf_size - s->is_null_terminated : 0;
		is_set = 1;
	}

	return is_set;
}

/* Set pointer to buf,
 * buf should be null-terminated string
 */
STRINGAA_INLINE int stringaa_set_chars(stringaa *s, const char *buf)
{
	int is_set = 0;
	size_t buf_size = 0;

	if (s && buf)
	{
		buf_size = strlen(buf);
		if (buf_size > 0)
		{
			is_set = stringaa_set_chars_size(s, buf, buf_size);
			if (is_set)
			{
				s->is_null_terminated = 1;
				s->buf_size++;
			}
		}
	}

	return is_set;
}

/* Set substring of s2 to s1
 * pos1: start position; flag STRINGAA_READ_POS: pos1 set at read position
 * pos2: end position + 1; flag STRINGAA_WRITE_POS: pos2 set at write position
 * size = pos2 - pos1
 */
STRINGAA_INLINE int stringaa_set_substring(stringaa *s1, stringaa *s2, int pos1, int pos2)
{
	if (pos1 == STRINGAA_READ_POS)
	{
		pos1 = s2->read_pos;
	}

	if (pos2 == STRINGAA_READ_POS)
	{
		pos2 = s2->read_pos;
	}
	else if (pos2 == STRINGAA_WRITE_POS)
	{
		pos2 = s2->write_pos;
	}

	if (pos1 >= 0 && pos1 < pos2 && pos2 <= s2->write_pos)
	{
		if (s1->is_alloc)
		{
			free(s1->buf);
		}

		s1->buf = (char*)s2->buf + pos1;
		s1->buf_size = pos2 - pos1;
		s1->read_pos = 0;
		s1->flags = STRINGAA_IS_INIT;
		s1->write_pos = s1->buf_size ? s1->buf_size - s1->is_null_terminated : 0;
	}

	return 1;
}

/* Set size of buf and copy 
 * copy buf_size bytes from buf to s
 */
STRINGAA_INLINE int stringaa_copy_chars_size(stringaa *s, const char *buf, const size_t buf_size)
{
	if (buf == NULL)
	{
		return 0;
	}

	if (!s->is_alloc || s->buf_size <= buf_size + 1)
	{
		stringaa_malloc(s, buf_size + 1);
	}

	memcpy(s->buf, buf, buf_size);
	s->write_pos = buf_size;
	if (s->is_null_terminated)
	{
		s->buf[s->write_pos] = 0;
	}
	s->read_pos = 0;
	if (s->is_init == 0)
	{
		s->is_init = 1;
	}

	return 1;
}

/* Set size of buf and copy,
 * buf should be null-terminated string 
 * copy from buf to s
 */
STRINGAA_INLINE int stringaa_copy_chars(stringaa *s, const char *buf)
{
	size_t buf_size = 0;
	int is_copy = 0;

	if (buf == NULL)
	{
		return 0;
	}

	buf_size = strlen(buf);

	is_copy = stringaa_copy_chars_size(s, buf, buf_size);

	return is_copy;
}

/* Set size of buf and copy
 * copy s2 to s1
 */
STRINGAA_INLINE int stringaa_copy(stringaa *s1, stringaa *s2)
{
	int is_copy = 0;

	if (s2->buf == NULL || s2->is_init == 0)
	{
		return 0;
	}

	is_copy = stringaa_copy_chars_size(s1, s2->buf, s2->buf_size - s2->is_null_terminated);
	if (is_copy)
	{
		s1->write_pos = s2->write_pos;
	}

	return is_copy;
}

/* Set size of buf and copy 
 * copy buf_size bytes from buf to s
 */
STRINGAA_INLINE int stringaa_add_chars_size(stringaa *s, const char *buf, const size_t buf_size)
{
	if (s && buf && buf_size)
	{
		if (s->is_alloc == 0 || s->buf_size <= s->write_pos + buf_size + 1)
		{
			stringaa_realloc(s, s->write_pos + buf_size + 1);
		}

		memcpy(s->buf + s->write_pos, buf, buf_size);
		s->write_pos += buf_size;
		if (s->is_null_terminated)
		{
			s->buf[s->write_pos] = 0;
		}
		s->read_pos = 0;
		if (s->is_init == 0)
		{
			s->is_init = 1;
		}
	}

	return 1;
}

/* Set size of buf and copy 
 * copy buf_size bytes from buf to s
 */
STRINGAA_INLINE int stringaa_add_chars(stringaa *s, const char *buf)
{
	int is_add = 0;

	if (s && buf)
	{
		is_add = stringaa_add_chars_size(s, buf, strlen(buf));
	}

	return is_add;
}

/* Set size of buf and copy 
 * copy buf_size bytes from buf to s
 */
STRINGAA_INLINE int stringaa_add(stringaa *s1, stringaa *s2)
{
	int is_add = 0;

	if (s1 && s2)
	{
		if (s2->buf && s2->is_init)
		{
			if (s2->read_pos < s2->write_pos &&
			s2->write_pos + s2->is_null_terminated <= s2->buf_size)
			{
				is_add = stringaa_add_chars_size(s1, s2->buf + s2->read_pos, s2->write_pos - s2->read_pos);
			}
		}
	}

	return is_add;
}

/* Return position of s_search in s, otherwise return -1
 * s: (in) string
 * s_search: (in) string to search in s
 */
STRINGAA_INLINE int stringaa_find(stringaa *s, stringaa *s_search)
{
	int is_find = -1;
	char c = 0;
	char c_search = 0;
	size_t pos = 0;
	int is_follow = 0;

	if (s && s_search)
	{
		if (s->buf && s->is_init &&
				s_search->buf && s_search->is_init)
		{
			stringaa_begin_read_pos(s);
			stringaa_begin_read_pos(s_search);

			c = stringaa_get_char(s);
			c_search = stringaa_get_char(s_search);
			while (c_search)
			{
				if (c == c_search)
				{
					if (!is_follow)
					{
						is_follow = 1;
						pos = s->read_pos;
					}
					if (!stringaa_next_read_pos(s_search))
					{
						is_find = pos;
						break;
					}
					c_search = stringaa_get_char(s_search);
				}
				else
				{
					if (is_follow)
					{
						is_follow = 0;
						stringaa_begin_read_pos(s_search);
						c_search = stringaa_get_char(s_search);
						s->read_pos = pos;
					}
				}
				if (!stringaa_next_read_pos(s))
				{
					break;
				}
				c = stringaa_get_char(s);
			}
		}
	}

	return is_find;
}

/* Clean s
 * set read_pos and write_pos to 0
 */
STRINGAA_INLINE int stringaa_clean(stringaa *s)
{
	int is_clean = 0;

	if (s)
	{
		if (stringaa_begin_read_pos(s))
		{
			if (stringaa_begin_write_pos(s))
			{
				is_clean = 1;
			}
		}
	}

	return is_clean;
}

/* Initialize struct
 */
STRINGAA_INLINE int stringaa_init(stringaa *s, size_t buf_size)
{
	int is_init = 0;

	if (s == NULL)
	{
		return 0;
	}

	memset(s, 0, sizeof(*s));
	
	is_init = 1;

	if (buf_size > 0)
	{
		is_init = stringaa_malloc(s, buf_size);
	}

	if (is_init)
	{
		s->is_init = 1;
	}

	return is_init;
}

/* Clean up struct
 */
STRINGAA_INLINE int stringaa_free(stringaa *s)
{
	int is_free = 1;

	/* We have next link
	 * Set next_link->buf_original = buf
	 * Cannot free buf
	 */
	if (s->next_link)
	{
		if (s->is_alloc)
		{
			/* alloc buf */
			s->next_link->buf_original = s->buf;
		}
		else
		{
			/* copy of buf */
			s->next_link->buf_original = s->buf_original;
		}
	}
	else if (s->is_alloc)
	{
		/* Can free buf */
		free(s->buf);
	}
	else if (s->buf_original)
	{
		/* copy of buf */
		free(s->buf_original);
	}

	s->buf = NULL;
	s->buf_size = 0;
	s->flags = 0;
	s->buf_original = NULL;
	s->read_pos = 0;
	s->write_pos = 0;

	return is_free;
}

#endif	/* STRINGAA_H */

