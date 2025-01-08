#ifndef DAY17_H_
#define DAY17_H_

#include "../dawn_utils.h"

#include <stddef.h>

typedef struct {
    size_t width;
    size_t length;
    size_t capacity;
    char **items;
} Map;

DawnStringBuilder get_map_data(char *filepath);
Map map_init(DawnStringBuilder map_data);
void map_free(Map map);

size_t get_sum_of_alignment_parameters(Map map);

#endif // DAY17_H_
