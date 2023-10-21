#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Deps {
    char** items;
    int length;
};

char* filedir(char* filepath) {
    char* filedir;
    int i = strlen(filepath) - 1;
    int lastSlashIndex = -1;

    while (lastSlashIndex == -1 && i >= 0) {
        if (filepath[i] == *"/") {
            lastSlashIndex = i;
        }

        i -= 1;
    }

    if (lastSlashIndex == -1) {
        return NULL;
    }

    filedir = calloc(lastSlashIndex + 1 + 1, sizeof(char));
    strncat(filedir, filepath, lastSlashIndex + 1);

    return filedir;
}

/**
 * Receives the line with no spaces
 */
char* findIncludePath(char* line, int lineLen) {
    char include[] = "#include\"";
    char includeLen = 9;
    char* path = calloc(1, sizeof(char));
    int pathLen = 0;

    /** The minimal line example to receive: #include"_.h" */
    if (lineLen < includeLen + 3 + 1) {
        return NULL;
    }

    for (int i = 0; i < lineLen; i++) {
        if (i > includeLen - 1) {
            if (line[i] == *"\"") {
                path[pathLen - 1] = *"c";

                return path;
            }

            path = realloc(path, pathLen + 2);

            strncat(path, &line[i], 1);
            pathLen += 1;
        } else if (line[i] != include[i]) {
            return NULL;
        }
    }

    free(path);

    return NULL;
}

struct Deps* parseFileDeps(char* filepath) {
    FILE* ptr;
    char ch;
    char* dir = filedir(filepath);
    struct Deps* deps = calloc(1, sizeof(struct Deps));
    char* line = calloc(1, sizeof(char));
    char* path;
    int lineLen = 0;
    int dirLen = 0;
    deps->length = 0;

    ptr = fopen(filepath, "r");

    if (ptr == NULL) {
        return NULL;
    }
    if (dir != NULL) {
        dirLen = strlen(dir);
    }

    do {
        ch = fgetc(ptr);

        if (ch == *"\n") {
            if (lineLen > 0) {
                if (line[0] != *"#") {
                    break;
                }

                char* includePath = findIncludePath(line, lineLen);

                if (includePath != NULL) {
                    char* relativePath = includePath;

                    if (dir != NULL) {
                        relativePath = calloc(strlen(includePath) + dirLen + 1, sizeof(char));
                        strcat(relativePath, dir);
                        strcat(relativePath, includePath);
                        free(includePath);
                    }

                    if (deps->length == 0) {
                        deps->items = calloc(1, sizeof(char*));
                    } else {
                        deps->items = realloc(deps->items, sizeof(char*) * (deps->length + 1));
                    }

                    deps->items[deps->length] = relativePath;

                    deps->length += 1;
                }
            }

            free(line);
            line = calloc(1, sizeof(char));
            lineLen = 0;
        } else if (ch != *" ") {
            line = realloc(line, lineLen + 2);
            strncat(line, &ch, 1);
            lineLen += 1;
        }

    } while (ch != EOF);

    free(line);
    fclose(ptr);

    return deps;
}
