RM = rm -rf
CC = gcc
CFLAGS = -std=c99 -g -D_GNU_SOURCE


default: all

all: trinity

trinity: trinity.c
	$(CC) $< $(CFLAGS) -otrinity

clean:
	$(RM) trinity

.PHONY: trinity clean
