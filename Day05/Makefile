CFLAGS = -Wall
CFLAGS += -Werror
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -pedantic-errors
CFLAGS += -std=c11
CFLAGS += -ggdb

../build/day5: day5.c intcode.c
	$(CC) -o $@ $^ $(CFLAGS)
