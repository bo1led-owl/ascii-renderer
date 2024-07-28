#include "src/cli.h"

#include <stdio.h>

#define COLOR_BOLD "\xB[1m"
#define COLOR_RESET "\xB[m"

CliResult check_arguments(int argc, const char** argv) {
    if (argc < 2) {
        return (CliResult){.status = Cli_TooFew};
    } else if (argc > 2) {
        return (CliResult){.status = Cli_TooMany};
    } else {
        return (CliResult){.status = Cli_Ok, .filename = argv[1]};
    }
}

void print_bold(const char* str) {
    printf("%s%s%s", COLOR_BOLD, str, COLOR_RESET);
}

void color_output_fg(stbi_uc r, stbi_uc g, stbi_uc b) {
    printf("\x1B[38;2;%u;%u;%um", r, g, b);  
}

void color_output_bg(stbi_uc r, stbi_uc g, stbi_uc b) {
    printf("\x1B[48;2;%u;%u;%um", r, g, b); 
}

void reset_color() { printf(COLOR_RESET); }
