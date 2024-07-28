#pragma once

#include <dep/stb_image.h>

typedef enum {
    Cli_Ok,
    Cli_TooFew,
    Cli_TooMany,
} CliStatus;

typedef struct {
    CliStatus status;
    const char* filename;
} CliResult;

CliResult check_arguments(int argc, const char** argv);
void print_bold(const char* str);
void color_output_fg(stbi_uc r, stbi_uc g, stbi_uc b);
void color_output_bg(stbi_uc r, stbi_uc g, stbi_uc b);
void reset_color();
