#include <stdlib.h>
#include <string.h>

char* arrangeFilename(char* filename) {
    int filenameLen = strlen(filename);

    if (filenameLen < 3 || filename[filenameLen - 2] != *"." || filename[filenameLen - 1] != *"c") {
        filename = realloc(filename, filenameLen + 3);

        strcat(filename, ".c");
    }

    return filename;
}