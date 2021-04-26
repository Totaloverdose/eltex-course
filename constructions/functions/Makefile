all: password test test.S

password: password.c
	gcc -O0 -Wall -fno-stack-protector password.c -o password

test: test.c
	gcc test.c -o test

test.S: test.c
	gcc -S test.c -o test.S

clean:
	rm test password test.S
