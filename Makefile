CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99 -Iinclude -g -fPIC -fvisibility=hidden

SRC=malloc.c find.c bit.c alloc_big.c alloc_buddy.c alloc_little.c
SRC:=$(addprefix src/, $(SRC))

BIN=libmalloc.so

OBJ=$(SRC:.c=.o)

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -shared $(CFLAGS) -o $@ $^

check:
	$(CC) $(CFLAGS) $(SRC) src/test.c

clean:
	$(RM) $(OBJ) $(BIN) vgcore.*

.PHONY: library lib clean check all
