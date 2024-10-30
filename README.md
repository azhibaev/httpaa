# Introduction

This is implementation of HTTP/1.1 and URI in C. The RFC 2616 and RFC 2396 is not newest, but there is plans to add the latest RFC.

I created simple server with asynchronous sockets and select() function. Server uses pool of workers. Multithreading features are based on pthread library.

Strings and buffers in memory is supported by special string library.

I used names for libraries, which contains of main title and suffix 'aa', it is my initials. For example, stringaa, httpaa, uriaa, socketaa, workeraa. This is simultaneously the types of data for certain structures.

The library simplifies creating of multipurpose server or client.

I write the library in GNU/Linux, and it has definitions for WIN32.

# The Socket Library

This is 'alpha' version of the Socket library. The Socket library is using the libraries: HTTP, URI, Worker and String.

The HTTP library works with HTTP/1.1 and implements RFC 2616. I know it's not a new RFC. I plan to add the newest RFC.
## Using the code

Every function of the library accepts a struct of data. For example, Server can be declared as a `socketaa` struct.
```C
socketaa server;

server.work.port = WORKERAA_HTTP_DEBUG_PORT;
server.work.pool.worker_count = WORKERAA_POOL_WORKER_COUNT_MIN + 1;
server.work.pool.work_size_max = WORKERAA_POOL_WORK_COUNT_MAX;
```
We can set properties of a server. The `port` is number port of listen socket. The `worker_count` is count of worker's threads. The `work_size_max` is size of works array.

Names of functions of socket library begins from 'socketaa' prefix. To start server it is need to call `socketaa_server()` function with a pointer to `socketaa` struct as an argument.
```C
socketaa_server(&server);
```

## Basic principles

The HTTP library can parse HTTP/1.1 request and fill `httpaa` struct. A request contains URI, which is parsed by the URI library.

A Server from the Socket library creates listen socket and waiting for a connection. When connection is accepted, then server fills a work struct and post the work in Workers Pool.

The Worker library contains a Workers Pool and Worker structs and functions.

A Worker is waiting for work. When a work is available, the Worker takes the work and begin to execute. The Worker determines a type of the work by a port number of the connection. There is some types of Workers.

A Worker for HTTP is type of Worker is suitable to work with HTTP request. The Worker receive data and process request with functions of HTTP library.

The String library handles strings and buffers for other libraries. Please note, that is alpha version, __so automatic free of buffers is not implemented__.

## Step by Step walk-throughs

The `socketaa_server()` function will initialize Workers Pool, creates listen socket and begins waiting for connection. When connection is accepted, `socketaa_server()` function fills `workeraa_work` struct and calls `workeraa_set_work()` function of Worker library.
```C
int workeraa_set_work(workeraa_pool *pool, void *p_work);
```
The `workeraa_set_work()` returns 0 on error, or 1 on success. The function locks mutex and copy `workeraa_work` struct to works array, then send signal to waiting worker, and unlocks mutex.
```C
if (!pthread_mutex_lock(&pool->sync->mutex))
{
	/* Add work */
	if (socketaa_set_work(pool->sync->work + pool->sync->work_write_index, p_work))
	{
		pool->sync->work_count++;
		pthread_cond_signal(&pool->sync->cond);
	}
	pthread_mutex_unlock(&pool->sync->mutex);
}
```
A worker is waiting for a work in `workeraa_get_work()` function.
```C
if (!pthread_mutex_lock(&worker->sync->mutex))
{
	/* Waiting for work */
	while (worker->sync->work_count == 0)
	{
		if (pthread_cond_wait(&worker->sync->cond, &worker->sync->mutex))
		{
			/* error */
			break;
		}
	}

	/* Get work */
	if (worker->sync->work_count > 0)
	{
		if (workeraa_work_check_read_index(worker))
		{
			worker->work.p_work = worker->sync->work[worker->sync->work_read_index].p_work;
			if (workeraa_work_set_next_read_index(worker))
			{
			}
			worker->sync->work_count--;
			is_get = 1;
		}
	}

	if (!pthread_mutex_unlock(&worker->sync->mutex))
	{
	}
}
```
When signal is received, `pthread_cond_wait()` function returns and worker gets pointer to `workeraa_work` struct with casting to void pointer because worker assigns work to the other type of worker.
```C
WORKERAA_INLINE int workeraa_func(workeraa *worker, 
		enum workeraa_func_enum n_func)
{
	int is_func = 0;

	if (worker)
	{
		switch (worker->work_id)
		{
			case WORKERAA_HTTP_PORT:
				is_func = workeraa_http_func(worker, n_func);
				break;
			case WORKERAA_HTTP_DEBUG_PORT:
				is_func = workeraa_http_func(worker, n_func);
				break;
			case WORKERAA_HTTPS_PORT:
				is_func = workeraa_https_func(worker, n_func);
				break;
			case WORKERAA_HTTPS_DEBUG_PORT:
				is_func = workeraa_https_func(worker, n_func);
				break;
			default:
				break;
		}
	}

	return is_func;
}
```
The `workeraa_func()` selects worker type by port number. If port number is `WORKERAA_HTTP_PORT` (80) or `WORKERAA_HTTP_DEBUG_PORT` (8080), then `workeraa_http` will be selected. By the way there is can be implemented different types of workers for every port number.

A Worker for HTTP will initialize `receive` and `send` buffers, and calls Socket worker for receiving and sending data over connection socket.

A Socket worker will waiting for data, copy it to worker's `receive` buffer and calls processing functions of Worker for HTTP.
```C
/* 1. recv HTTP request
 * 2. parse request */
WORKERAA_INLINE int workeraa_http_recv(workeraa *worker)
{
	worker_http = (workeraa_http*)worker->work.p_struct;
	/* Parse HTTP header */
	if (workeraa_http_header(worker))
	{
		/* HTTP header is parsed,
		 * execute specific actions for certain method */
		workeraa_http_method(worker, worker_http->http.request.request_line.method);
		if (worker_http->http.request.request_line.method == http11_method_get)
		{
			/* If GET method then set SEND flag */
			socketaa_set_work_flags(&worker->work, SOCKETAA_WORK_FLAGS_IS_SEND | SOCKETAA_WORK_FLAGS_IS_SELECT);
		}
	}
}
```
The `workeraa_http_recv()` function sets `SEND` flag if HTTP header is received and parsed.

A Socket worker will call `workeraa_http_send()` function and send data over connection socket.
```C
WORKERAA_INLINE int workeraa_http_send(workeraa *worker)
{
	worker_http = (workeraa_http*)worker->work.p_struct;
	if (worker_http->is_header_send == 0)
	{
		if (workeraa_http_create_response(worker))
		{
			worker_http->is_header_send = 1;
		}
	}
}
```
# Conclusion and Points of Interest

Initially this library was writing for WebRTC server. Actually the library can be used for different types of servers. This is 'alpha' version, so not all features are implemented. I will update the library. Stay tuned!

The 'string' library is very interesting. It has some mathematics functions. Have a fun! In the next articles I will write how to work with them.
