#pragma once

#include <dep/stb_image.h>

typedef struct {
    stbi_uc* data;
    int width;
    int height;
} Image;

void render_grayscale(Image image);
void render_rgb(Image image);
void render_rgba(Image image);
