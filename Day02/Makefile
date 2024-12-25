CFLAGS = -Wall
CFLAGS += -Werror
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -pedantic-errors
CFLAGS += -std=c11
CFLAGS += -ggdb

../build/day2_p1: day2_p1.c intcode.c
	$(CC) -o $@ $^ $(CFLAGS)

../build/day2_p2: day2_p2.c intcode.c
	$(CC) -o $@ $^ $(CFLAGS)
