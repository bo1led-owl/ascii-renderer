#include <assert.h>
#include <dep/stb_image.h>
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

static void print_help(void) {
    print_bold("Usage");
    puts(": " STR(PROJECT_NAME) " <IMAGE>");
    puts("");

    print_bold("Arguments");
    puts(":");
    puts("  IMAGE: path to the image to be rendered in ascii");
    puts("");

    print_bold("Flags");
    puts(":");
    puts("  --help: see this message");
    puts("  -g, --grayscale: render image in grayscale");
}

static bool is_flag(const char* s) {
    assert(strlen(s) > 0);
    return s[0] == '-';
}

static Args handle_flag(const char* s, Args args) {
    if (strcmp(s, "--help") == 0) {
        print_help();
        exit(0);
    } else if (strcmp(s, "-g") == 0 || strcmp(s, "--grayscale") == 0) {
        args.grayscale = true;
    } else {
        fprintf(stderr, "Unknown flag, use --help to see the guide\n");
        exit(2);
    }
    return args;
}

static Args handle_cli(int argc, const char** argv) {
    Args args = (Args){0};
    for (int i = 1; i < argc; ++i) {
        if (is_flag(argv[i])) {
            args = handle_flag(argv[i], args);
        } else {
            if (args.filename != NULL) {
                fprintf(
                    stderr,
                    "Too many arguments passed, use --help to see the guide\n");
                exit(2);
            }

            args.filename = argv[i];
        }
    }

    if (args.filename == NULL) {
        fprintf(stderr,
                "Too few arguments passed, use --help to see the guide\n");
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
