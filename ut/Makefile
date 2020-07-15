SRC = $(wildcard list/*.c) test.c

CFLAGS = -Wall -std=c99 -I../ -I./
CFLAGS += `pkg-config --libs cunit`

test: ${SRC} ../macro.h
	${CC} ${CFLAGS} -o $@ ${SRC}

.PHONY: clean
clean:
	rm -f test
