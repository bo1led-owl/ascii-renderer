#include "src/render.h"

#include <assert.h>
#include <math.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "src/cli.h"

typedef struct {
    stbi_uc r, g, b;
} RgbPixel;

typedef struct {
    stbi_uc r, g, b, a;
} RgbaPixel;

static const char characters[] =
    // " !\"#$%&\()*+,-./"
    // "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
    // "abcdefghijklmnopqrstuvwxyz{|}~";
    "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/"
    "\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

enum {
    CHAR_COUNT = sizeof(characters),
};

static char get_char_by_luminance(const double luminance) {
    assert(CHAR_COUNT == sizeof(characters));

    const int char_index = CHAR_COUNT - 1 - floor(luminance * CHAR_COUNT);
    assert(char_index >= 0);
    assert(char_index < CHAR_COUNT);
    return characters[char_index];
}

static char quantize_grayscale(const Image image, const int x, const int y,
                               const int pixels_per_char) {
    unsigned sum = 0;
    for (int i = y; i < pixels_per_char; ++i) {
        for (int j = x; j < pixels_per_char; ++j) {
            sum += image.data[i * image.width + j];
        }
    }

    const stbi_uc average_value = sum / (pixels_per_char * pixels_per_char);
    return get_char_by_luminance(average_value / (double)256);
}

static RgbPixel quantize_rgb(const Image image, const int x, const int y,
                             const int pixels_per_char) {
    unsigned sum_r = 0;
    unsigned sum_g = 0;
    unsigned sum_b = 0;

    const RgbPixel* pixels = (const RgbPixel*)image.data;

    for (int i = y; i < y + pixels_per_char; ++i) {
        for (int j = x; j < x + pixels_per_char; ++j) {
            sum_r += pixels[i * image.width + j].r;
            sum_g += pixels[i * image.width + j].g;
            sum_b += pixels[i * image.width + j].b;
        }
    }

    const int pixels_per_char_squared = pixels_per_char * pixels_per_char;
    return (RgbPixel){
        .r = sum_r / pixels_per_char_squared,
        .g = sum_g / pixels_per_char_squared,
        .b = sum_b / pixels_per_char_squared,
    };
}

static RgbaPixel quantize_rgba(const Image image, const int x, const int y,
                               const int pixels_per_char) {
    unsigned sum_r = 0;
    unsigned sum_g = 0;
    unsigned sum_b = 0;
    unsigned sum_a = 0;

    const RgbaPixel* pixels = (const RgbaPixel*)image.data;

    for (int i = y; i < y + pixels_per_char; ++i) {
        for (int j = x; j < x + pixels_per_char; ++j) {
            sum_r += pixels[i * image.width + j].r;
            sum_g += pixels[i * image.width + j].g;
            sum_b += pixels[i * image.width + j].b;
            sum_a += pixels[i * image.width + j].a;
        }
    }

    const int pixels_per_char_squared = pixels_per_char * pixels_per_char;
    return (RgbaPixel){
        .r = sum_r / pixels_per_char_squared,
        .g = sum_g / pixels_per_char_squared,
        .b = sum_b / pixels_per_char_squared,
        .a = sum_a / pixels_per_char_squared,
    };
}

static int get_pixels_per_char(const int image_width, const int image_height) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    const int horizontal = image_width / w.ws_col;
    const int vertical = image_height / w.ws_row;
    return (horizontal > vertical) ? horizontal : vertical;
}

void render_grayscale(Image image) {
    const int pixels_per_char = get_pixels_per_char(image.width, image.height);

    for (int i = 0; i < image.height; i += pixels_per_char) {
        for (int j = 0; j < image.width; j += pixels_per_char) {
            const stbi_uc pixel =
                quantize_grayscale(image, j, i, pixels_per_char);
            const double luminance = pixel / 256.0;

            color_output_fg(pixel, pixel, pixel);
            putchar(get_char_by_luminance(luminance));
        }
    }
}

void render_rgb(Image image) {
    const int pixels_per_char = get_pixels_per_char(image.width, image.height);

    for (int i = 0; i + pixels_per_char < image.height; i += pixels_per_char) {
        for (int j = 0; j + pixels_per_char < image.width;
             j += pixels_per_char) {
            const RgbPixel pixel = quantize_rgb(image, j, i, pixels_per_char);
            const double luminance =
                (pixel.r / 256.0 + pixel.b / 256.0 + pixel.g / 256.0) / 3.0;

            color_output_fg(pixel.r, pixel.g, pixel.b);
            putchar(get_char_by_luminance(luminance));
        }
        reset_color();
        putchar('\n');
    }

    reset_color();
}

void render_rgba(Image image) {
    const int pixels_per_char = get_pixels_per_char(image.width, image.height);

    for (int i = 0; i + pixels_per_char < image.height; i += pixels_per_char) {
        for (int j = 0; j + pixels_per_char < image.width;
             j += pixels_per_char) {
            const RgbaPixel pixel = quantize_rgba(image, j, i, pixels_per_char);
            const double luminance =
                (pixel.r / 256.0 + pixel.b / 256.0 + pixel.g / 256.0) / 3.0 *
                (pixel.a / 256.0);

            color_output_fg(pixel.r, pixel.g, pixel.b);
            putchar(get_char_by_luminance(luminance));
        }
        reset_color();
        putchar('\n');
    }

    reset_color();
}
