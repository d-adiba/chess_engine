FILES := $(wildcard *.[c])


all:
	gcc -oFast $(FILES)  -o drie

debug:
	gcc -Wall -Wextra -Werror $(FILES) -o drie

