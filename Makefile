macro.h: make.sh src/*
	./make.sh

.PHONY: test
test: macro.h
	${MAKE} -C ut
	./ut/test

.PHONY: clean
clean:
	rm macro.h
	${MAKE} -C ut clean
