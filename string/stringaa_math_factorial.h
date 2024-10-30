/* String Library. Mathematics functions.
 * Copyright (C) 2021  Askar Azhibaev
 *
 * File: stringaa_math_factorial.h
 * Description: Mathematics functions for string library
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: December 2020
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef STRINGAA_MATH_FACTORIAL_H
#define STRINGAA_MATH_FACTORIAL_H

#include "stringaa_math.h"

/* Multiplies n odds 
 * example: 1 * 3 * 5 * 7 * 9 ; n = 9
 */
STRINGAA_INLINE int stringaa_mul_odd_n_int(int n)
{
	int c = 1;
	int i = 1;

	for (i = 1; i <= n; i += 2)
	{
		c = c * i;
	}

	return c;
}

/* Multiplies n odds but m 
 * example: 1 * 3 * 7 * 9 ; n = 9, m = 5
 */
STRINGAA_INLINE int stringaa_mul_odd_n_m_int(int n, int m)
{
	int c = 1;
	int i = 1;

	for (i = 1; i <= n; i += 2)
	{
		if (i != m)
		{
			c = c * i;
		}
	}

	return c;
}

/* Multiplies n odds 
 * result in s
 */
STRINGAA_INLINE int stringaa_mul_odd_n(stringaa *s, stringaa *n)
{
	stringaa c = {0};
	stringaa c1 = {0};
	stringaa i = {0};
	stringaa i_inc = {0};

	stringaa_set_string(&i, "1");
	stringaa_set_string(&i_inc, "2");

	while (stringaa_compare(&i, n) <= 0)
	{
		stringaa_add(&i, &i_inc);
		{
			stringaa_mul(&c1, &c, &i);
			stringaa_copy(&c, &c1);
		}
	}

	return 1;
}

/* Multiplies n odds but m 
 * result in s
 */
STRINGAA_INLINE int stringaa_mul_odd_n_m_string(stringaa *s, stringaa *n, stringaa *m)
{
	stringaa c = {0};
	stringaa c1 = {0};
	stringaa i = {0};
	stringaa i_inc = {0};

	stringaa_set_string(&i, "1");
	stringaa_set_string(&i_inc, "2");
	stringaa_set_string(&c, "1");

	while (stringaa_compare(&i, n) <= 0)
	{
		stringaa_add(&i, &i_inc);
		printf("i = ");
		stringaa_print(&i);
		printf("\ni_inc = ");
		stringaa_print(&i_inc);
		printf("\n");
		{
			if (stringaa_compare(&i, m) != 0)
			{
				stringaa_mul(&c1, &c, &i);
		printf("c = ");
		stringaa_print(&c);
		printf("\n");
		printf("c1 = ");
		stringaa_print(&c1);
		printf("\n");
				stringaa_copy(&c, &c1);
		printf("copy c = ");
		stringaa_print(&c);
		printf("\n");
			}
		}
	}

	return 1;
}

#endif	/* STRINGAA_MATH_FACTORIAL_H */

