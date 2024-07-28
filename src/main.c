#include <assert.h>
#include <stb_image.h>
#include <stdio.h>
#include <string.h>

#include "src/cli.h"
#include "src/render.h"

#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)

#ifndef PROJECT_NAME
#error PROJECT_NAME not defined, cannot generate help message
#endif

static void print_help() {
    print_bold("Usage");
    printf(": " STR(PROJECT_NAME) " <IMAGE>");
    printf("\n\n");

    print_bold("Arguments");
    printf(":\n");
    printf("\tIMAGE - path to the image to be rendered in ascii");
    printf("\n\n");

    print_bold("Flags");
    printf(":\n");
    printf("\t--help: see this message");
    printf("\n");
}

static const char* handle_cli(int argc, const char** argv) {
    CliResult result = check_arguments(argc, argv);
    switch (result.status) {
        case Cli_TooFew:
            fprintf(stderr,
                    "Too few arguments passed, use --help to see the guide\n");
            exit(2);
        case Cli_TooMany:
            fprintf(stderr,
                    "Too many arguments passed, use --help to see the guide\n");
            exit(2);
        case Cli_Ok:
            if (strcmp(argv[1], "--help") == 0) {
                print_help();
                exit(0);
            } else {
                return argv[1];
            }
    }
    return NULL;
}

int main(const int argc, const char** argv) {
    const char* filename = handle_cli(argc, argv);
    assert(filename);

    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Could not open image file");
        exit(2);
    }

    Image image = {0};
    int channels;
    image.data =
        stbi_load_from_file(file, &image.width, &image.height, &channels, 0);
    if (!image.data) {
        fprintf(stderr, "Could not load the image: %s:", stbi_failure_reason());
        exit(2);
    }

    int exit_code = 0;
    switch (channels) {
        case 1:
            render_grayscale(image);
            break;
        case 3:
            render_rgb(image);
            break;
        case 4:
            render_rgba(image);
            break;
        default:
            fprintf(stderr,
                    "Cannot process image with %d color channels, only images "
                    "with 1, 3 or 4 channels are supported\n",
                    channels);
            exit_code = 2;
            break;
    }

    stbi_image_free(image.data);
    return exit_code;
}
