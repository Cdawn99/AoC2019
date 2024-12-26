CFLAGS = -Wall
CFLAGS += -Werror
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -pedantic-errors
CFLAGS += -std=c11
CFLAGS += -ggdb

../build/day3_p1: day3_p1.c day3.c
	$(CC) -o $@ $^ $(CFLAGS)

../build/day3_p2: day3_p2.c day3.c
	$(CC) -o $@ $^ $(CFLAGS)
