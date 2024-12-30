CFLAGS = -Wall
CFLAGS += -Werror
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -pedantic-errors
CFLAGS += -std=c11
CFLAGS += -ggdb

../build/day7_p1: day7_p1.c intcode.c
	$(CC) -o $@ $^ $(CFLAGS)
