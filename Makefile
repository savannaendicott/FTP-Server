C = gcc -Wall -std=c11 -D_POSIX_SOURCE -o

all:	client server

client:	client.c header.h
	$(C) client client.c wrappers.c 

server:	server.c header.h
	$(C) server server.c wrappers.c

clean:	
	rm -f client server
