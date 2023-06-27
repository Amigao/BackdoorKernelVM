CC = gcc
CFLAGS = -Wall

all: client

client: client.c
	$(CC) $(CFLAGS) -o client client.c -lws2_32

clean:
	rm -f client