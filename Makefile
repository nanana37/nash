CC = gcc
CFLAGS = 
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
EXEC = nash

all: $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

debug: CFLAGS += -g -DDEBUG
debug: all

clean:
	rm -f $(OBJS) $(EXEC)