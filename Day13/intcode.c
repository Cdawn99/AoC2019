#include "intcode.h"

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 32
#define EXTRA_MEM_SIZE 1024

static void extend_program(Program *p) {
    int64_t extra_mem[EXTRA_MEM_SIZE] = {0};
    DAWN_DA_APPEND_MANY(p, extra_mem, EXTRA_MEM_SIZE);
}

Program read_program(char *filepath) {
    DawnStringBuilder content = {0};
    if (!dawn_read_entire_file(filepath, &content)) return (Program){0};
    DAWN_SB_APPEND_BUF(&content, "", 1);

    Program p = {0};
    char delim[] = ",\n";
    char *token = strtok(content.items, delim);
    while (token) {
        int64_t instruction = atol(token);
        DAWN_DA_APPEND(&p, instruction);
        token = strtok(NULL, delim);
    }
    extend_program(&p);

    DAWN_SB_FREE(content);
    return p;
}

void free_program(Program p) {
    DAWN_DA_FREE(p);
}

void print_instruction(Program *p, size_t n) {
    printf("%zu: ", p->ip);
    for (size_t ip = p->ip; ip < p->ip + n; ip++) {
        printf("%" PRId64 " ", p->items[ip]);
    }
    putchar('\n');
}

static size_t program_add(Program *p, int64_t arg_modes) {
    int64_t s = 0;
    for (size_t ip = p->ip + 1; ip < p->ip + 3; ip++) {
        switch (arg_modes % 10) {
            case 0: {
                s += p->items[p->items[ip]];
            } break;
            case 1: {
                s += p->items[ip];
            } break;
            case 2: {
                s += p->items[p->rel_base + p->items[ip]];
            } break;
        }
        arg_modes /= 10;
    }
    size_t offset = arg_modes == 2 ? p->rel_base : 0;
    p->items[offset + p->items[p->ip + 3]] = s;
    return 4;
}

static size_t program_mul(Program *p, int64_t arg_modes) {
    int64_t m = 1;
    for (size_t ip = p->ip + 1; ip < p->ip + 3; ip++) {
        switch (arg_modes % 10) {
            case 0: {
                m *= p->items[p->items[ip]];
            } break;
            case 1: {
                m *= p->items[ip];
            } break;
            case 2: {
                m *= p->items[p->rel_base + p->items[ip]];
            } break;
        }
        arg_modes /= 10;
    }
    size_t offset = arg_modes == 2 ? p->rel_base : 0;
    p->items[offset + p->items[p->ip + 3]] = m;
    return 4;
}

static size_t program_read(Program *p, int64_t arg_mode) {
    char buff[BUFFER_SIZE] = {0};
    if (!fgets(buff, BUFFER_SIZE, stdin)) {
        perror("Failed to read input");
        goto defer;
    }
    size_t offset = arg_mode == 2 ? p->rel_base : 0;
    p->items[offset + p->items[p->ip + 1]] = atol(buff);
defer:
    return 2;
}

static size_t program_print(Program *p, int64_t arg_mode, char *buffer) {
    switch (arg_mode) {
        case 0: {
            sprintf(buffer, "%" PRId64 "\n", p->items[p->items[p->ip + 1]]);
        } break;
        case 1: {
            sprintf(buffer, "%" PRId64 "\n", p->items[p->ip + 1]);
        } break;
        case 2: {
            sprintf(buffer, "%" PRId64 "\n", p->items[p->rel_base + p->items[p->ip + 1]]);
        } break;
    }
    return 2;
}

static size_t program_jnz(Program *p, int64_t arg_modes) {
    int64_t params[2];
    for (size_t i = 1; i < 3; i++) {
        switch (arg_modes % 10) {
            case 0: {
                params[i - 1] = p->items[p->items[p->ip + i]];
            } break;
            case 1: {
                params[i - 1] = p->items[p->ip + i];
            } break;
            case 2: {
                params[i - 1] = p->items[p->rel_base + p->items[p->ip + i]];
            } break;
        }
        arg_modes /= 10;
    }
    return params[0] != 0 ? (size_t)params[1] : p->ip + 3;
}

static size_t program_jz(Program *p, int64_t arg_modes) {
    int64_t params[2];
    for (size_t i = 1; i < 3; i++) {
        switch (arg_modes % 10) {
            case 0: {
                params[i - 1] = p->items[p->items[p->ip + i]];
            } break;
            case 1: {
                params[i - 1] = p->items[p->ip + i];
            } break;
            case 2: {
                params[i - 1] = p->items[p->rel_base + p->items[p->ip + i]];
            } break;
        }
        arg_modes /= 10;
    }
    return params[0] == 0 ? (size_t)params[1] : p->ip + 3;
}

static size_t program_lt(Program *p, int64_t arg_modes) {
    int64_t params[2];
    for (size_t i = 1; i < 3; i++) {
        switch (arg_modes % 10) {
            case 0: {
                params[i - 1] = p->items[p->items[p->ip + i]];
            } break;
            case 1: {
                params[i - 1] = p->items[p->ip + i];
            } break;
            case 2: {
                params[i - 1] = p->items[p->rel_base + p->items[p->ip + i]];
            } break;
        }
        arg_modes /= 10;
    }
    size_t offset = arg_modes == 2 ? p->rel_base : 0;
    p->items[offset + p->items[p->ip + 3]] = params[0] < params[1] ? 1 : 0;
    return 4;
}

static size_t program_eq(Program *p, int64_t arg_modes) {
    int64_t params[2];
    for (size_t i = 1; i < 3; i++) {
        switch (arg_modes % 10) {
            case 0: {
                params[i - 1] = p->items[p->items[p->ip + i]];
            } break;
            case 1: {
                params[i - 1] = p->items[p->ip + i];
            } break;
            case 2: {
                params[i - 1] = p->items[p->rel_base + p->items[p->ip + i]];
            } break;
        }
        arg_modes /= 10;
    }
    size_t offset = arg_modes == 2 ? p->rel_base : 0;
    p->items[offset + p->items[p->ip + 3]] = params[0] == params[1] ? 1 : 0;
    return 4;
}

static size_t program_adjust_rel_base(Program *p, int64_t arg_mode) {
    switch (arg_mode) {
        case 0: {
            p->rel_base += p->items[p->items[p->ip + 1]];
        } break;
        case 1: {
            p->rel_base += p->items[p->ip + 1];
        } break;
        case 2: {
            p->rel_base += p->items[p->rel_base + p->items[p->ip + 1]];
        } break;
    }
    return 2;
}

DawnStringBuilder run_program(Program *p) {
    DawnStringBuilder output = {0};
    char buffer[BUFFER_SIZE] = {0};
    while (p->items[p->ip] != 99) {
        switch (p->items[p->ip] % 100) {
            case 1: {
                // print_instruction(p, 4);
                p->ip += program_add(p, p->items[p->ip] / 100);
            } break;
            case 2: {
                // print_instruction(p, 4);
                p->ip += program_mul(p, p->items[p->ip] / 100);
            } break;
            case 3: {
                // print_instruction(p, 2);
                p->ip += program_read(p, p->items[p->ip] / 100);
            } break;
            case 4: {
                // print_instruction(p, 2);
                p->ip += program_print(p, p->items[p->ip] / 100, buffer);
                DAWN_SB_APPEND_CSTR(&output, buffer);
                memset(buffer, 0, BUFFER_SIZE);
            } break;
            case 5: {
                // print_instruction(p, 3);
                p->ip = program_jnz(p, p->items[p->ip] / 100);
            } break;
            case 6: {
                // print_instruction(p, 3);
                p->ip = program_jz(p, p->items[p->ip] / 100);
            } break;
            case 7: {
                // print_instruction(p, 4);
                p->ip += program_lt(p, p->items[p->ip] / 100);
            } break;
            case 8: {
                // print_instruction(p, 4);
                p->ip += program_eq(p, p->items[p->ip] / 100);
            } break;
            case 9: {
                // print_instruction(p, 2);
                p->ip += program_adjust_rel_base(p, p->items[p->ip] / 100);
            } break;
        }
    }
    // print_instruction(p, 1);
    return output;
}

void print_program(Program p) {
    printf("%" PRId64 "", p.items[0]);
    for (size_t i = 1; i < p.length; i++) {
        printf(",%" PRId64 "", p.items[i]);
    }
    putchar('\n');
}
