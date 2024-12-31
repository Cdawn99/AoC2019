#ifndef DAY8_H_
#define DAY8_H_

#include "../dawn_utils.h"

#include <stddef.h>

typedef struct {
    size_t width;
    size_t height;

    size_t length;
    size_t capacity;
    char **items;
} ImageData;

ImageData image_data_init(DawnStringBuilder image_data, size_t width, size_t height);
void image_data_free(ImageData image);

size_t image_data_get_min_digit_count_layer(ImageData image, char digit);
size_t image_data_get_layer_digit_count(ImageData image, size_t layer, char digit);

typedef struct {
    size_t width;
    size_t height;
    char **data;
} Image;

Image image_init(ImageData data);
void image_free(Image image);
void image_print(Image image);

#endif // DAY8_H_
