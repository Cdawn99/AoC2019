#include "day14.h"
#include "../dawn_utils.h"

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t name;
    size_t quantity;
} Material;

typedef struct {
    size_t length;
    size_t capacity;
    Material *items;
} Materials;

struct Reaction {
    Material output;
    Materials input;
};

static size_t parse_quantity(char *info) {
    size_t quantity = 0;
    while ('0' <= *info && *info <= '9') {
        quantity *= 10;
        quantity += *info - '0';
        info++;
    }
    return quantity;
}

static size_t parse_name(char *info) {
    size_t name = 0;
    while ('A' <= *info && *info <= 'Z') {
        name *= 27;
        name += *info - 'A' + 1;
        info++;
    }
    return name;
}

static char *parse_input(Materials *input, char *info) {
    while (*info != '=') {
        Material m = {0};
        m.quantity = parse_quantity(info);
        while (*info < 'A' || 'Z' < *info) info++;
        m.name = parse_name(info);
        while (*info != '=' && (*info < '0' || '9' < *info)) info++;
        DAWN_DA_APPEND(input, m);
    }
    return info;
}

Material parse_output(char *info) {
    Material m = {0};
    while (*info < '0' || '9' < *info) info++;
    m.quantity = parse_quantity(info);
    while (*info < 'A' || 'Z' < *info) info++;
    m.name = parse_name(info);
    return m;
}

Reactions reactions_init(char *filepath) {
    Reactions rs = {0};
    DawnStringBuilder content = {0};
    if (!dawn_read_entire_file(filepath, &content)) return rs;
    DAWN_SB_APPEND_BUF(&content, "", 1);
    char delim[] = "\n";
    char *token = strtok(content.items, delim);
    while (token) {
        Reaction r = {0};
        token = parse_input(&r.input, token);
        r.output = parse_output(token);
        DAWN_DA_APPEND(&rs, r);
        token = strtok(NULL, delim);
    }
    return rs;
}

void reactions_free(Reactions rs) {
    for (size_t i = 0; i < rs.length; i++) {
        DAWN_DA_FREE(rs.items[i].input);
    }
    DAWN_DA_FREE(rs);
}

size_t compute_ore_requirement(Reactions rs) {
    Materials processing_q = {0};
    for (size_t i = 0; i < rs.length; i++) {
        Material m = rs.items[i].output;
        m.quantity = 0;
        DAWN_DA_APPEND(&processing_q, m);
    }

    for (size_t i = processing_q.length - 1; i > 0; i--) {
        bool swapped = true;
        while (swapped) {
            Material product = processing_q.items[i];
            Reaction *recipe = rs.items;
            while (recipe->output.name != product.name) recipe++;
            for (size_t j = i - 1; j < processing_q.length; j--) {
                bool is_input = false;
                for (size_t k = 0; k < recipe->input.length; k++) {
                    if (recipe->input.items[k].name == processing_q.items[j].name) {
                        is_input = true;
                        break;
                    }
                }
                if (is_input) {
                    Material temp = processing_q.items[i];
                    processing_q.items[i] = processing_q.items[j];
                    processing_q.items[j] = temp;
                    swapped = true;
                    break;
                }
                swapped = false;
            }
        }
    }
    assert(processing_q.items[0].name == parse_name("FUEL"));
    processing_q.items[0].quantity = 1;

    size_t ore = parse_name("ORE");
    Material m_ore = { .name = ore };
    DAWN_DA_APPEND(&processing_q, m_ore);

    for (size_t i = 0; i < processing_q.length - 1; i++) {
        Material product = processing_q.items[i];

        Reaction *recipe = rs.items;
        while (recipe->output.name != product.name) recipe++;

        size_t f = ceil((double)product.quantity/recipe->output.quantity);

        for (size_t j = 0; j < recipe->input.length; j++) {
            Material material = recipe->input.items[j];

            Material *pqm = processing_q.items;
            while (pqm->name != material.name) pqm++;

            pqm->quantity += material.quantity * f;
        }
    }

    size_t ore_amount = processing_q.items[processing_q.length - 1].quantity;
    DAWN_DA_FREE(processing_q);
    return ore_amount;
}
