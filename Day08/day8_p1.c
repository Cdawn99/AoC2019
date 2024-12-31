#include "day8.h"

#define DAWN_IMPLEMENTATION
#include "../dawn_utils.h"

#include <ctype.h>
#include <stdio.h>

#define WIDTH 25
#define HEIGHT 6

int main(int argc, char **argv) {
    char *program = dawn_shift_args(&argc, &argv);
    if (argc == 0) {
        printf("Usage: %s <image_data.txt>\n", program);
        return 1;
    }

    char *fp = dawn_shift_args(&argc, &argv);
    DawnStringBuilder image_data = {0};
    if (!dawn_read_entire_file(fp, &image_data)) return 1;
    while (isspace(image_data.items[image_data.length - 1])) image_data.length--;

    ImageData im = image_data_init(image_data, WIDTH, HEIGHT);

    size_t min_zeros_layer = image_data_get_min_digit_count_layer(im, '0');
    size_t ones_count = image_data_get_layer_digit_count(im, min_zeros_layer, '1');
    size_t twos_count = image_data_get_layer_digit_count(im, min_zeros_layer, '2');

    printf("Number of ones times number of twos: %zu\n", ones_count * twos_count);

    image_data_free(im);
    DAWN_SB_FREE(image_data);

    return 0;
}
