#ifndef INTCODE_H_
#define INTCODE_H_

#include "../dawn_utils.h"

#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t ip;

    size_t capacity;
    size_t length;
    int64_t *items;
} Program;

Program read_program(char *filepath);
void free_program(Program p);

DawnStringBuilder run_program(Program *p);
void print_program(Program p);

#endif // INTCODE_H_
