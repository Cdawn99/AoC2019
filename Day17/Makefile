CFLAGS = -Wall
CFLAGS += -Werror
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -pedantic-errors
CFLAGS += -std=c11
CFLAGS += -ggdb

../build/day17_p1: day17_p1.c day17.c intcode.c
	$(CC) -o $@ $^ $(CFLAGS)
