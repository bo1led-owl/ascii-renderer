#include "src/cli.h"

#include <stdio.h>

#define COLOR_BOLD "\x1B[1m"
#define COLOR_RESET "\x1B[0m"

void print_bold(const char* str) {
    printf("%s%s%s", COLOR_BOLD, str, COLOR_RESET);
}

void color_output_fg(stbi_uc r, stbi_uc g, stbi_uc b) {
    printf("\x1B[38;2;%u;%u;%um", r, g, b);
}

void color_output_bg(stbi_uc r, stbi_uc g, stbi_uc b) {
    printf("\x1B[48;2;%u;%u;%um", r, g, b);
}

void reset_color(void) { printf(COLOR_RESET); }
