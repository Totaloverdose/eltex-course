main: main.c ../interface.h
	gcc main.c -o main -lrt

test: main.c ../interface.h
	gcc -g -O0 main.c -o test -lrt

clean:
	rm main test
