SRC = $(wildcard list/*.c)

CFLAGS = -Wall -DUT_TEST_BASIC -I../ -I./
CFLAGS += `pkg-config --libs cunit`

test: test.c ${SRC}
	${CC} ${CFLAGS} -o $@ $^

.PHONY: clean
clean:
	rm -f test
