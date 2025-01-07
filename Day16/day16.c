#include "day16.h"
#include "../dawn_utils.h"

#include <assert.h>
#include <stdlib.h>

#define ARRAY_LEN(arr) (sizeof((arr))/sizeof((arr)[0]))

List list_init(char *filepath) {
    List list = {0};
    DawnStringBuilder content = {0};
    if (!dawn_read_entire_file(filepath, &content)) return list;
    while (content.items[content.length - 1] < '0' || '9' < content.items[content.length - 1]) content.length--;
    for (size_t i = 0; i < content.length; i++) {
        int64_t n = content.items[i] - '0';
        DAWN_DA_APPEND(&list, n);
    }
    DAWN_SB_FREE(content);
    return list;
}

void list_free(List list) {
    DAWN_DA_FREE(list);
}

static inline void list_swap(List *l1, List *l2) {
    List temp = *l1;
    *l1 = *l2;
    *l2 = temp;
}

static inline List list_copy(List list) {
    List copy = {0};
    DAWN_DA_APPEND_MANY(&copy, list.items, list.length);
    return copy;
}

void list_repeat(List *list, size_t repeat_count) {
    size_t list_len = list->length;
    for (size_t i = 0; i < repeat_count; i++) {
        DAWN_DA_APPEND_MANY(list, list->items, list_len);
    }
}

int64_t extract_offset(List list) {
    int64_t offset = 0;
    for (size_t i = 0; i < 7; i++) {
        offset *= 10;
        offset += list.items[i];
    }
    return offset;
}

void flawed_frequency_transmission(List *signal, size_t phase_count) {
    static int64_t base_pattern[] = {1, 0, -1, 0};
    static size_t base_pattern_len = ARRAY_LEN(base_pattern);

    List next = list_copy(*signal);
    memset(next.items, 0, sizeof(*next.items) * next.length);

    for (size_t phase = 0; phase < phase_count; phase++) {
        for (size_t i = 0; i < signal->length; i++) {
            size_t pattern_idx = 0;
            size_t until_next_pattern = i + 1;
            size_t pattern_count = i + 1;
            for (size_t j = i; j < signal->length; j++) {
                next.items[i] += signal->items[j] * base_pattern[pattern_idx];
                if (--until_next_pattern == 0) {
                    pattern_idx = (pattern_idx + 1) % base_pattern_len;
                    until_next_pattern = pattern_count;
                }
            }
            if (next.items[i] < 0) next.items[i] *= -1;
            next.items[i] %= 10;
        }
        list_swap(signal, &next);
        memset(next.items, 0, sizeof(*next.items) * next.length);
    }

    list_free(next);
}
