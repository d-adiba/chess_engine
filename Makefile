CC =  gcc
CFLAGS = -Ofast  -DVERSION=\"$(shell git describe --tags --always 2>/dev/null || echo 'dev')\"
SRCS = bitboard.c move.c uci.c random.c attacks.c  chipolata.c
OBJS = $(SRCS:.c=.o)


all: chipolata

chipolata: $(OBJS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) chipolata

.PHONY: all clean

