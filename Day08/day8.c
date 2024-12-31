#include "day8.h"

#include <stdint.h>

ImageData image_data_init(DawnStringBuilder image_data, size_t width, size_t height) {
    ImageData image = {0};
    image.width = width;
    image.height = height;
    for (size_t i = 0; i < image_data.length; i += width * height) {
        char *it = image_data.items + i;
        DAWN_DA_APPEND(&image, it);
    }
    return image;
}

void image_data_free(ImageData image) {
    DAWN_DA_FREE(image);
}

size_t image_data_get_min_digit_count_layer(ImageData image, char digit) {
    size_t layer = 0;
    size_t min_digit_count = SIZE_MAX;
    for (size_t i = 0; i < image.length; i++) {
        size_t layer_digit_count = 0;
        for (size_t j = 0; j < image.width * image.height; j++) {
            if (image.items[i][j] == digit) layer_digit_count++;
        }
        if (min_digit_count > layer_digit_count) {
            min_digit_count = layer_digit_count;
            layer = i;
        }
    }
    return layer;
}

size_t image_data_get_layer_digit_count(ImageData image, size_t layer, char digit) {
    char *it = image.items[layer];
    size_t count = 0;
    for (size_t i = 0; i < image.width * image.height; i++) {
        if (it[i] == digit) count++;
    }
    return count;
}
