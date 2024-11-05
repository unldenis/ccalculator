CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OUTFILE = output
OBJS = main.o
SRCS = main.c

$(OUTFILE): $(OBJS)
		$(CC) $(CFLAGS) -o $(OUTFILE) $(OBJS)
$(OBJS): $(SRCS)
		$(CC) $(CFLAGS) -c $(SRCS)
