#include "s2html_conv.h"
#include <stdio.h>
#include <string.h>

static void usage(const char *program)
{
    fprintf(stderr, "Usage: %s <input.c> [output.html]\n", program);
}

int main(int argc, char **argv)
{
    const char *input_path;
    char default_output[512];
    const char *output_path;
    int result;

    if (argc < 2 || argc > 3) {
        usage(argv[0]);
        return 1;
    }

    input_path = argv[1];
    if (argc == 3) {
        output_path = argv[2];
    } else {
        size_t n = strlen(input_path);
        if (n + 6 >= sizeof(default_output)) {
            fprintf(stderr, "Input path is too long.\n");
            return 1;
        }
        snprintf(default_output, sizeof(default_output), "%s.html", input_path);
        output_path = default_output;
    }

    result = s2html_convert_file(input_path, output_path);
    if (result != 0) {
        fprintf(stderr, "Conversion failed.\n");
        return 1;
    }

    printf("Created HTML file: %s\n", output_path);
    return 0;
}
