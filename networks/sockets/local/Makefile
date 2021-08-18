all: client server

client: client.c ../interface.h
	gcc -Wall -Wextra -g -O0 client.c -o client

server: server.c ../interface.h
	gcc -Wall -Wextra -g -O0 server.c -o server

clean:
	rm client server
