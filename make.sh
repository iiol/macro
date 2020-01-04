#!/bin/sh

HDRS="$(echo src/includes.h; ls src/*.h | grep -v 'src/includes.h')"
SRCS="$(ls src/*.c)"

cat $HDRS $SRCS | grep -v "^#include \"" >macro.h
