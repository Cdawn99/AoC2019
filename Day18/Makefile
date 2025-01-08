CFLAGS = -Wall
CFLAGS += -Werror
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -pedantic-errors
CFLAGS += -std=c11
CFLAGS += -ggdb

../build/day18_count_keys: count_keys.c day18.c
	$(CC) -o $@ $^ $(CFLAGS)

../build/day18_p1: day18_p1.c day18.c
	$(CC) -o $@ $^ $(CFLAGS)

../build/day18_p2: day18_p2.c day18.c
	$(CC) -o $@ $^ $(CFLAGS)
