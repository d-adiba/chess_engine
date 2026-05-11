CC =  gcc
CFLAGS = -Ofast -g -mbmi2 -DVERSION=\"$(shell git describe --tags --always 2>/dev/null || echo 'dev')\"
SRCS = bitboard.c move.c uci.c random.c attacks.c  chipolata.c atomic/move_atomic.c atomic/eval.c
OBJS = $(SRCS:.c=.o)


all: chipolata

chipolata: $(OBJS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

fclean:
	rm -f $(OBJS)
clean: fclean
	rm -f chipolata

.PHONY: all clean

