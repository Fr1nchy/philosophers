CC=gcc
CFLAGS=-Wall -g -lm
LDLIBS=-lpthread

all: philosophers

clean:
	$(RM) philosophers *.o
