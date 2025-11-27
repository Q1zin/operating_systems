CC      = gcc
CFLAGS  = -Wall -Wextra -fPIC -fsanitize=address -fsanitize=leak
LDFLAGS = -pthread
LIB     = libmythread.so

SRCS    = mythread.c
OBJS    = $(SRCS:.c=.o)

.PHONY: all clean

all: $(LIB) $(TESTS)

$(LIB): $(OBJS)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

%.o: %.c mythread.h
	$(CC) $(CFLAGS) -c $< -o $@

%: %.c $(LIB) mythread.h
	$(CC) $(CFLAGS) $< -L. -lmythread -Wl,-rpath,. $(LDFLAGS) -o $@

clean:
	rm -f *.o $(LIB) $(TESTS)
