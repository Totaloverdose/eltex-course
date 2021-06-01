

main: main.c print_error.c print_error.h
	gcc main.c print_error.c -o main -lncurses -ldl

test: main.c print_error.c print_error.h
	gcc -g -O0 main.c print_error.c -o test -lncurses -ldl

clean:
	rm test main
