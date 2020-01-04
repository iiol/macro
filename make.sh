#!/bin/sh

OUTPUT="macro.h"

>$OUTPUT

(echo src/includes.h; ls src/*.h | grep -v 'src/includes.h') |
while read filename; do
	echo "// File: $filename" >>$OUTPUT
	cat "$filename" | tail -n +3 | head -n -1 | grep -v '^#include "' |
	    awk '!/^$/ || ISPRINT {print; ISPRINT=1}' >>$OUTPUT
done

ls src/*.c |
while read filename; do
	echo "// File: $filename" >>$OUTPUT
	cat "$filename" | grep -v '^#include "' |
	    awk '!/^$/ || ISPRINT {print; ISPRINT=1}' >>$OUTPUT
done
