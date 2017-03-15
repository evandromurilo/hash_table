C = gcc
CFLAGS = -I ../lib 
OUT = -o ../bin/test_hash

PROG = test_hash
OBJS = test_hash.o hash.o ../lib/m_basics.o
SRCS = test_hash.c hash.c ../lib/m_basics.c
DEPS = ../lib/m_basics.h hash.h

%.o: %.c $(DEPS)
	        $(CC) $(DEBUG) -c -o $@ $< $(CFLAGS)

$(PROG): $(OBJS)
	        $(CC) $(DEBUG) $(OUT) $(CFLAGS) $(OBJS)

run:
	        ../bin/test_hash

debug: DEBUG = -DDEBUG
debug: $(PROG)

