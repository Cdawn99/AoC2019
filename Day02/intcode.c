#include "intcode.h"
#include "../dawn_utils.h"

#include <stdlib.h>
#include <string.h>

Program read_program(char *filepath) {
    DawnStringBuilder content = {0};
    if (!dawn_read_entire_file(filepath, &content)) return (Program){0};
    DAWN_SB_APPEND_BUF(&content, "", 1);

    Program p = {0};
    char delim[] = ",\n";
    char *token = strtok(content.items, delim);
    while (token) {
        int64_t instruction = atoi(token);
        DAWN_DA_APPEND(&p, instruction);
        token = strtok(NULL, delim);
    }

    DAWN_SB_FREE(content);
    return p;
}

void free_program(Program p) {
    DAWN_DA_FREE(p);
}

void run_program(Program *p) {
    while (p->items[p->ip] != 99) {
        switch (p->items[p->ip]) {
            case 1: {
                int64_t i1 = p->items[p->ip + 1];
                int64_t i2 = p->items[p->ip + 2];
                int64_t i3 = p->items[p->ip + 3];

                int64_t n1 = p->items[i1];
                int64_t n2 = p->items[i2];

                p->items[i3] = n1 + n2;
            } break;
            case 2: {
                int64_t i1 = p->items[p->ip + 1];
                int64_t i2 = p->items[p->ip + 2];
                int64_t i3 = p->items[p->ip + 3];

                int64_t n1 = p->items[i1];
                int64_t n2 = p->items[i2];

                p->items[i3] = n1 * n2;
            } break;
        }
        p->ip += 4;
    }
}

void print_program(Program p) {
    printf("%ld", p.items[0]);
    for (size_t i = 1; i < p.length; i++) {
        printf(",%ld", p.items[i]);
    }
    putchar('\n');
}
