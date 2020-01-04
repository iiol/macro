test: test.c macro.h
	$(CC) -Wall -o test test.c

macro.h: make.sh src/*
	./make.sh
