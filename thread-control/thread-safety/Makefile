LIBPATH := .

all: libmy_strerror.so main

libmy_strerror.so: my_strerror.c
	gcc -fPIC -shared my_strerror.c -o libmy_strerror.so

main: main.c my_strerror.h
	gcc main.c -o main -pthread -L $(LIBPATH) -l my_strerror -Wl,-rpath,$(LIBPATH)

test: main.c my_strerror.h
	gcc -O0 -g main.c -o test

clean:
	rm main test libmy_strerror.so
