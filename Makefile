FILES:= $(wildcard *.[c])

all:
	gcc $(FILES)  -o drie 
fast:
	gcc $(FILES) -Ofast -o drie
gdb: 	
	gcc -g $(FILES) -o drie
debug:
	gcc -Wall -Wextra -Werror  $(FILES) -o drie
