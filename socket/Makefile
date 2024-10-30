CFLAGS=-Wall -D_DEBUG -I. -I../string -I../url -I../http -I../worker -g
LIBS=-lpthread
SOURCES=test.c
DEPS=socketaa* ../url/uriaa* ../http/httpaa* ../string/stringaa* ../worker/workeraa*

test: $(SOURCES) $(DEPS)
	cc $(CFLAGS) $(LIBS) $(SOURCES) -o $@

