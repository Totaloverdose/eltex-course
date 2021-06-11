

main: main.c
	gcc main.c -o main -pthread

test: main.c
	gcc -g -O0 main.c -o test -pthread

clean:
	rm test main
