CFLAGS = -Wall
CFLAGS += -Werror
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -pedantic-errors
CFLAGS += -std=c11
CFLAGS += -ggdb

../build/day16_p1: day16_p1.c day16.c
	$(CC) -o $@ $^ $(CFLAGS)

../build/day16_p2: day16_p2.c day16.c
	$(CC) -o $@ $^ $(CFLAGS)
