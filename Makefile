CC = gcc
CFLAGS = -lm

all:	idk

idk: main.c
	$(CC) main.c -o idk $(CFLAGS)

clean:
	rm -f idk
