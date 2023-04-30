#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

typedef unsigned char byte;
#define LOG(...) fprintf(stderr, __VA_ARGS__)

int main(int argc, char **argv) {
    char *image_file;
    FILE *out = stdout;

    assert(argc > 0);
    if (argc < 2) {
        LOG("USAGE: %s <PNG file> [out_filename]\n", argv[0]);
        return 1;
    } else if (argc > 3) {
        LOG("ERROR: Extra arguments (%d):", argc - 2);
        for (int i = 2; i < argc; i++) {
            LOG(" %s", argv[i]);
        }
        return 1;
    } else {
        assert(argc <= 3);
        switch (argc) {
            case 3:
                out = fopen(argv[2], "w");
                if (!out) {
                    LOG("%s: could not open '%s': %s\n",
                        argv[0], argv[2], strerror(errno)
                    );
                    return 1;
                }
            case 2:
                image_file = argv[1];
        }
    }

    int x, y, n;
    byte *data = stbi_load(image_file, &x, &y, &n, 1);
    if (!data) {
        LOG("%s: %s\n", image_file, stbi_failure_reason());
        return 1;
    }

    fprintf(out, "%d %d $", y, x);
    for (int j = 0; j < y; j++) {
        for (int i = 0; i < x; i++) {
            const int index = j * x + i;
            const byte pixel = data[index];
            fprintf(out, " %hhu", pixel);
        }
    }

    fclose(out);
    stbi_image_free(data);
    return 0;
}
