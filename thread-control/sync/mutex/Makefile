main: main.c
	gcc main.c -o main -pthread

test: main.c
	gcc -O0 -g main.c -o test -pthread

clean:
	rm main test
