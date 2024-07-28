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

typedef struct {
    const char* filename;
    bool grayscale;
} Args;

static void print_help() {
    print_bold("Usage");
    printf(": " STR(PROJECT_NAME) " <IMAGE>");
    printf("\n\n");

    print_bold("Arguments");
    printf(":\n");
    printf("\tIMAGE: path to the image to be rendered in ascii");
    printf("\n\n");

    print_bold("Flags");
    printf(":\n");
    printf("\t--help: see this message\n");
    printf("\t-g, --grayscale: render image in grayscale\n");
}

static bool is_flag(const char* s) {
    size_t dash_count = 0;
    for (char const* c = s; *c != '\0'; ++c) {
        if (*c == '-') {
            dash_count += 1;
        } else {
            break;
        }
    }

    return dash_count > 0;
}

static void handle_flag(const char* s, Args* args) {
    if (strcmp(s, "--help") == 0) {
        print_help();
        exit(0);
    } else if (strcmp(s, "-g") == 0 || strcmp(s, "--grayscale") == 0) {
        args->grayscale = true;
    } else {
        fprintf(stderr, "Unknown flag, use --help to see the guide\n");
        exit(2);
    }
}

static Args handle_cli(int argc, const char** argv) {
    Args args = {0};
    size_t arg_count = 0;
    for (int i = 1; i < argc; ++i) {
        if (is_flag(argv[i])) {
            handle_flag(argv[i], &args);
        } else {
            arg_count++;

            args.filename = argv[i];
        }
    }

    if (arg_count < 1) {
        fprintf(stderr,
                "Too few arguments passed, use --help to see the guide\n");
        exit(2);
    } else if (arg_count > 1) {
        fprintf(stderr,
                "Too many arguments passed, use --help to see the guide\n");
        exit(2);
    }

    return args;
}

int main(const int argc, const char** argv) {
    const Args args = handle_cli(argc, argv);

    FILE* file = fopen(args.filename, "r");
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
            render_rgb(image, args.grayscale);
            break;
        case 4:
            render_rgba(image, args.grayscale);
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
