#include "day8.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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

Image image_init(ImageData data) {
    char *image_data = malloc((data.width + 1) * data.height * sizeof(*image_data) + 1);
    assert(image_data && "Buy more RAM lol");
    memset(image_data, ' ', (data.width + 1) * data.height * sizeof(*image_data) + 1);
    image_data[(data.width + 1) * data.height] = '\0';
    for (size_t i = 0; i < data.height; i++) {
        image_data[i * (data.width + 1) + data.width] = '\n';
    }
    char **image = malloc(data.height * sizeof(*image));
    assert(image && "Buy more RAM lol");
    for (size_t i = 0; i < data.height; i++) {
        image[i] = image_data + i * (data.width + 1);
    }
    for (size_t i = data.length - 1; i < data.length; i--) {
        char *it = data.items[i];
        for (size_t j = 0; j < data.height; j++) {
            for (size_t k = 0; k < data.width; k++) {
                switch (it[j * data.width + k]) {
                    case '0': /* Black */
                        image[j][k] = '.';
                        break;
                    case '1': /* White */
                        image[j][k] = '#';
                        break;
                    case '2': /* Transparent */
                        break;
                }
            }
        }
    }
    return (Image){
        .height = data.height,
        .width = data.width,
        .data = image,
    };
}

void image_free(Image image) {
    if (image.data) free(image.data[0]);
    free(image.data);
}

void image_print(Image image) {
    puts(image.data[0]);
}
