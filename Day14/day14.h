#ifndef DAY14_H_
#define DAY14_H_

#include <stddef.h>

typedef struct Reaction Reaction;

typedef struct {
    size_t length;
    size_t capacity;
    Reaction *items;
} Reactions;

Reactions reactions_init(char *filepath);
void reactions_free(Reactions rs);

size_t compute_ore_requirement(Reactions rs);

#endif // DAY14_H_
