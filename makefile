CC = gcc
CFLAGS = -Wall -Wextra

all: clean manager feed

manager: 
	$(CC) $(CFLAGS) -o manager manager/manager.c -pthread

feed: 
	$(CC) $(CFLAGS) -o feed feed/feed.c  


clean:
	rm -f feed
	rm -f manager
	rm -f $(wildcard pipe_cliente*)


.PHONY: all clean manager feed
