CFLAGS=-Wall -I../solver -I../socket -I../string -I../url -I../http -g -D_DEBUG
LDFLAGS=
LIBS=-pthread
SOURCES=test.c
DEPS=*.h ../solver ../socket ../string ../url ../http

test: $(SOURCES) $(DEPS)
	cc $(CFLAGS) $(LDFLAGS) $(LIBS) $(SOURCES) -o $@

