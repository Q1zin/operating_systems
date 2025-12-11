CC      = gcc
CFLAGS  = -Wall -Wextra -fPIC -fsanitize=address
LDFLAGS = 
LIB     = libuthread.so

SRCS    = uthread.c
OBJS    = $(SRCS:.c=.o)

TEST_SRCS = $(wildcard tests/*.c)
TESTS     = $(patsubst tests/%.c, %, $(TEST_SRCS))

.PHONY: all clean tests

all: $(LIB) $(TESTS)

$(LIB): $(OBJS)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

%.o: %.c uthread.h
	$(CC) $(CFLAGS) -c $< -o $@

%: %.c $(LIB) uthread.h
	$(CC) $(CFLAGS) $< -L. -luthread -Wl,-rpath,. $(LDFLAGS) -o $@

%: tests/%.c $(LIB) uthread.h
	$(CC) $(CFLAGS) $< -L. -luthread -Wl,-rpath,. $(LDFLAGS) -o $@

clean:
	rm -f *.o $(LIB) $(TESTS)