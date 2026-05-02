FILES:= $(wildcard *.[c])
.silent: fast gdb debug bgrecord
MAKEFLAGS += --no-print-directory

all:
	@gcc $(FILES)  -o drie
fast:
	@gcc $(FILES) -Ofast -o drie
gdb: 	
	@gcc -g $(FILES) -o drie
debug:
	@gcc -Wall -Wextra -Werror  $(FILES) -o drie
pgo:
	@gcc -Ofast -fprofile-generate -march=native $(FILES) -o drie
	@./drie
	@gcc -Ofast -fprofile-use -march=native $(FILES) -o drie
bgrecord:
	@echo "Program drie lancé en arrière plan resultats enregistrés dans bgrecord.txt"
	@(make fast && ./drie > bgrecord.txt 2>&1 && echo "program terminé") &

