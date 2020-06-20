SRC = $(wildcard list/*.c)

CFLAGS = -Wall -I../ -I./
CFLAGS += `pkg-config --libs cunit`

test: test.c ${SRC}
	${CC} ${CFLAGS} -o $@ $^

.PHONY: clean
clean:
	rm -f test
