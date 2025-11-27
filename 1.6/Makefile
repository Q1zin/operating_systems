CC      = gcc
CFLAGS  = -Wall -Wextra -fPIC -fsanitize=address -fsanitize=leak
LDFLAGS = -pthread
LIB     = libmythread.so

SRCS    = mythread.c
OBJS    = $(SRCS:.c=.o)

TEST_SRCS = $(wildcard tests/1.2/*.c)
TESTS     = $(patsubst tests/1.2/%.c, 1_2_%, $(TEST_SRCS))

.PHONY: all clean tests

all: $(LIB) $(TESTS)

$(LIB): $(OBJS)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

%.o: %.c mythread.h
	$(CC) $(CFLAGS) -c $< -o $@

%: %.c $(LIB) mythread.h
	$(CC) $(CFLAGS) $< -L. -lmythread -Wl,-rpath,. $(LDFLAGS) -o $@

1_2_%: tests/1.2/%.c $(LIB) mythread.h
	$(CC) $(CFLAGS) $< -L. -lmythread -Wl,-rpath,. $(LDFLAGS) -o $@

clean:
	rm -f *.o $(LIB) $(TESTS)
