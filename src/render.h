#pragma once

#include <dep/stb_image.h>
#include <stdbool.h>

typedef struct {
    stbi_uc* data;
    int width;
    int height;
} Image;

void render_grayscale(Image image);
void render_rgb(Image image, bool grayscale);
void render_rgba(Image image, bool grayscale);
