CC      = gcc
CFLAGS  = -Wall -Wextra -fPIC -fsanitize=address -fsanitize=leak
LDFLAGS = -pthread
LIB     = libmythread.so

SRCS    = mythread.c
OBJS    = $(SRCS:.c=.o)

TEST_SRCS = $(wildcard tests/*.c)
TESTS     = $(patsubst tests/%.c, %, $(TEST_SRCS))

.PHONY: all clean tests

all: $(LIB) $(TESTS)

$(LIB): $(OBJS)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

%.o: %.c mythread.h
	$(CC) $(CFLAGS) -c $< -o $@

%: %.c $(LIB) mythread.h
	$(CC) $(CFLAGS) $< -L. -lmythread -Wl,-rpath,. $(LDFLAGS) -o $@

%: tests/%.c $(LIB) mythread.h
	$(CC) $(CFLAGS) $< -L. -lmythread -Wl,-rpath,. $(LDFLAGS) -o $@

clean:
	rm -f *.o $(LIB) $(TESTS)
