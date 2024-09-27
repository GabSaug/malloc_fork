CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99 -Iinclude -O0 -fPIC -fvisibility=hidden -fno-inline $(EXTRA_CFLAGS)

SRC=malloc.c find.c bit.c alloc_big.c alloc_buddy.c alloc_little.c malloc_no_mutex.c
SRC:=$(addprefix src/, $(SRC))

BIN=build/bin/malloc

OBJ=$(SRC:.c=.o)

all: $(BIN)

$(BIN): $(OBJ)
	mkdir -p build/bin
	$(CC) -shared $(CFLAGS) -o $@ $^

check: $(BIN)
	@sh -c "cd tests && ./tests.sh"

clean:
	$(RM) $(OBJ) $(BIN) vgcore.*

.PHONY: library lib clean check all
