all: client server

client: client.c interface.h
	gcc -g -O0 -Wall -Wextra client.c -o client 
	
server: server.c interface.h
	gcc -g -O0 -Wall -Wextra server.c -o server -pthread
	
	
clean:
	rm client server