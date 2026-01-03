CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Ilib

build:
	$(CC) $(CFLAGS) src/*.c -o app

clean:
	rm -f app
