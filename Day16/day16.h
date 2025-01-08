#ifndef DAY16_H_
#define DAY16_H_

#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t length;
    size_t capacity;
    int8_t *items;
} List;

List list_init(char *filepath);
void list_free(List list);
void list_repeat(List *list, size_t repeat_count);

size_t extract_offset(List list);
void flawed_frequency_transmission(List *signal, size_t phase_count);

#endif // DAY16_H_
