/* Worker library. Pool of pthread workers.
 * Copyright (C) 2021  Askar Azhibaev
 * 
 * File: workeraa_index_utils.h
 * Description: Worker library
 * Author: Askar Azhibaev
 * E-mail: <askar@9o1d.com>
 * Date: February 2021
 * License: The Code Project Open License (CPOL)
 * License URL: https://www.codeproject.com/info/cpol10.aspx
 */

#ifndef __WORKERAA_INDEX_UTILS_H
#define __WORKERAA_INDEX_UTILS_H

/* Set next read position
 * 1	if (read == write)
 * 1.1		if (count > 0)
 * 1.1.1		if (read + 1 < size)	read++;	is_set = 1	-> (3)
 * 1.1.2		else
 * 1.1.2.1			if (write > 0)	read = 0;	is_set = 1	-> (2)
 * 1.1.2.2			else	read = size	-> (3)
 * 2	if (read < write)
 * 2.1		if (read + 1 < write)	read++;	is_set = 1	-> (2)
 * 2.2		else	read = write	-> (1)
 * 3	if (read > write)
 * 3.1		if (read + 1 < size)	read++;	is_set = 1	-> (3)
 * 3.2		else
 * 3.2.1			if (write > 0)	read = 0;	is_set = 1	-> (2)
 * 3.2.2			else	read = size	-> (3)
 */
WORKERAA_INLINE size_t workeraa_work_set_next_read_index(workeraa *worker)
{
	int is_set = 0;
	size_t read_index = 0;
	size_t write_index = 0;
	size_t work_count = 0;
	size_t work_size = 0;

	if (worker)
	{
		if (worker->sync)
		{
			read_index = worker->sync->work_read_index;
			write_index = worker->sync->work_write_index;
			work_count = worker->sync->work_count;
			work_size = worker->sync->work_size;

			if (read_index > work_size)
			{
				read_index = work_size;
			}
			if (write_index > work_size)
			{
				write_index = work_size;
			}

			if (read_index < write_index)
			{
				if (read_index + 1 < write_index)
				{
					read_index++;
					is_set = 1;
				}
				else
				{
					read_index = write_index;
				}
				worker->sync->work_read_index = read_index;
			}
			else if (read_index > write_index 
					|| ((read_index == write_index) 
						&& work_count > 0))
			{
				if (read_index + 1 < work_size)
				{
					read_index++;
					is_set = 1;
				}
				else
				{
					if (write_index > 0)
					{
						read_index = 0;
						is_set = 1;
					}
					else
					{
						read_index = work_size;
					}
				}
				worker->sync->work_read_index = read_index;
			}
		}
	}

	return is_set;
}

/* Set next write position
 * 1	if (write == read)
 * 1.1		if (count == 0)
 * 1.1.1		if (write + 1 < size)	write++;	is_set = 1
 * 1.2			else
 * 1.2.1			if (read > 0)	write = 0;	is_set = 1
 * 1.2.2			else	write = size
 * 2	if (write > read)
 * 2.1		if (write + 1 < size)	write++;	is_set = 1
 * 2.2		else
 * 2.2.1		if (read > 0)	write = 0;	is_set = 1
 * 2.2.2		else	write = size
 * 3	if (write < read)
 * 3.1		if (write + 1 < read)	write++;	is_set = 1
 * 3.2		else	write = read
 */
WORKERAA_INLINE size_t workeraa_work_set_next_write_index(workeraa_pool *pool)
{
	int is_set = 0;
	size_t read_index = 0;
	size_t write_index = 0;
	size_t work_count = 0;
	size_t work_size = 0;

	if (pool)
	{
		if (pool->sync)
		{
			read_index = pool->sync->work_read_index;
			write_index = pool->sync->work_write_index;
			work_count = pool->sync->work_count;
			work_size = pool->sync->work_size;

			if (read_index > work_size)
			{
				read_index = work_size;
			}
			if (write_index > work_size)
			{
				write_index = work_size;
			}

			if (write_index > read_index
					|| ((write_index == read_index)
						&& work_count == 0))
			{
				if (write_index + 1 < work_size)
				{
					write_index++;
					is_set = 1;
				}
				else
				{
					if (read_index > 0)
					{
						write_index = 0;
						is_set = 1;
					}
					else
					{
						write_index = work_size;
					}
				}
				pool->sync->work_write_index = write_index;
			}
			if (write_index < read_index)
			{
				if (write_index + 1 < read_index)
				{
					write_index++;
					is_set = 1;
				}
				else
				{
					write_index = read_index;
				}
				pool->sync->work_write_index = write_index;
			}
		}
	}

	return is_set;
}

/* Check read index 
 * 1	if (read == write)
 * 1.1		if (count > 0)
 * 1.1.1		if (read < size)	is_checked = 1
 * 2	if (read < write)	is_checked = 1
 * 3	if (read > write)
 * 3.1		if (read < size)	is_checked = 1
 * 3.2		else
 * 3.2.1		if (write > 0)	read = 0;	is_checked = 1
 */
WORKERAA_INLINE int workeraa_work_check_read_index(workeraa *worker)
{
	int is_checked = 0;
	size_t read_index = 0;
	size_t write_index = 0;
	size_t work_count = 0;
	size_t work_size = 0;

	if (worker)
	{
		if (worker->sync)
		{
			read_index = worker->sync->work_read_index;
			write_index = worker->sync->work_write_index;
			work_count = worker->sync->work_count;
			work_size = worker->sync->work_size;

			if (read_index > work_size)
			{
				read_index = work_size;
			}
			if (write_index > work_size)
			{
				write_index = work_size;
			}

			if (read_index == write_index
					&& work_count > 0
					&& read_index < work_size)
			{
				is_checked = 1;
			}
			else if (read_index < write_index)
			{
				is_checked = 1;
			}
			else if (read_index > write_index)
			{
				if (read_index < work_size)
				{
					is_checked = 1;
				}
				else if (write_index > 0)
				{
					read_index = 0;
					is_checked = 1;
				}
			}
		}
	}

	return is_checked;
}

/* Check write index
 * 1	if (write == read)
 * 1.1		if (count == 0)
 * 1.1.1		if (write < size)	is_checked = 1
 * 2	if (write > read)
 * 2.1		if (write < size)	is_checked = 1
 * 2.2		else
 * 2.2.1		if (read > 0)	write = 0;	is_checked = 1
 * 3	if (write < read)	is_checked = 1
 */
WORKERAA_INLINE int workeraa_work_check_write_index(workeraa_pool *pool)
{
	int is_checked = 0;
	size_t read_index = 0;
	size_t write_index = 0;
	size_t work_count = 0;
	size_t work_size = 0;

	if (pool)
	{
		if (pool->sync)
		{
			read_index = pool->sync->work_read_index;
			write_index = pool->sync->work_write_index;
			work_count = pool->sync->work_count;
			work_size = pool->sync->work_size;

			if (read_index > work_size)
			{
				read_index = work_size;
			}
			if (write_index > work_size)
			{
				write_index = work_size;
			}

			if (write_index == read_index
					&& work_count == 0
					&& write_index < work_size)
			{
				is_checked = 1;
			}
			else if (write_index > read_index)
			{
				if (write_index < work_size)
				{
					is_checked = 1;
				}
				else if (read_index > 0)
				{
					write_index = 0;
					is_checked = 1;
				}
			}
			else if (write_index < read_index)
			{
				is_checked = 1;
			}
		}
	}

	return is_checked;
}

#endif	/* __WORKERAA_INDEX_UTILS_H */

