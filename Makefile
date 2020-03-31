CC=gcc
CFLAGS=-I

all: server client

server: src/servidor.c
	$(CC) -o build/server src/servidor.c

client: src/cliente.c
	$(CC) -o build/client src/cliente.c

.PHONY: clean

clean:
	@ -rm -f build/*