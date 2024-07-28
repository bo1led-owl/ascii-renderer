#pragma once

#include <dep/stb_image.h>

void print_bold(const char* str);
void color_output_fg(stbi_uc r, stbi_uc g, stbi_uc b);
void color_output_bg(stbi_uc r, stbi_uc g, stbi_uc b);
void reset_color();
