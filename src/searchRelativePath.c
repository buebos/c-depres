#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "util/printe.h"

struct DirStack {
    int capacity;
    int len;
    DIR** items;
};

struct DirStack* DirStack(int capacity) {
    struct DirStack* stack = (struct DirStack*)malloc(sizeof(struct DirStack));
    stack->capacity = capacity;
    stack->items = malloc(stack->capacity * sizeof(DIR*));
    stack->len = 0;
    return stack;
}

int isFull(struct DirStack* stack) {
    return stack->len == stack->capacity;
}

DIR* getTop(struct DirStack* stack) {
    if (stack->len == 0) {
        return NULL;
    }

    return stack->items[stack->len - 1];
}

DIR* push(struct DirStack* stack, DIR* dir) {
    if (isFull(stack)) {
        printf("[ERROR] Can't push dir, the stack is full: %s\n", dir->dd_dir);

        return NULL;
    }

    stack->items[stack->len] = dir;
    stack->len += 1;

    return stack->items[stack->len - 1];
}

DIR* pop(struct DirStack* stack) {
    DIR* topDir = getTop(stack);

    if (topDir == NULL) {
        printe("The stack is empty, cannot remove anything");

        return NULL;
    }

    closedir(topDir);
    stack->len -= 1;

    return getTop(stack);
}

char* genNewTopDirPath(struct DirStack* dirs) {
    char* path = calloc(1, sizeof(char));

    for (int i = 0; i < dirs->len; i++) {
        char* dirName = dirs->items[i]->dd_dta.name;
        int dirNameLen = strlen(dirName);

        if (!dirNameLen) {
            continue;
        }

        path = realloc(path, strlen(path) + dirNameLen + 2);

        strcat(path, dirName);
        strcat(path, "/");
    }

    return path;
}

int isValidDir(const char* path) {
    struct stat statbuf;

    if (stat(path, &statbuf) != 0 || path[0] == *".") {
        return 0;
    }

    return S_ISDIR(statbuf.st_mode);
}

/**
 * Searches for the file relative path given the file name
 */
char* searchRelativePath(char* filename) {
    char* currentPath = calloc(1, sizeof(char));
    struct dirent* dp;
    struct DirStack* dirs = DirStack(50);
    DIR* top = opendir(".");

    if (top == NULL) {
        printe("Could not open root directory. Are there enough permissions for the exe?");
        return NULL;
    }

    push(dirs, top);

    dp = readdir(top);

    while (dp != NULL) {
        if (strcmp(dp->d_name, filename) == 0) {
            currentPath = realloc(currentPath, strlen(currentPath) + strlen(filename) + 1);

            strcat(currentPath, filename);

            return currentPath;
        }

        char* dpPath = calloc(strlen(currentPath) + strlen(dp->d_name) + 1, sizeof(char));

        strcat(dpPath, currentPath);
        strcat(dpPath, dp->d_name);

        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && isValidDir(dpPath)) {
            free(currentPath);

            currentPath = genNewTopDirPath(dirs);
            top = opendir(currentPath);

            if (top == NULL) {
                printf("[ERROR] Could not open directory: %s. Are there enough permissions for the exe?", currentPath);
                free(currentPath);
                return NULL;
            }

            push(dirs, top);
        }

        free(dpPath);

        dp = readdir(top);

        if (dp == NULL) {
            top = pop(dirs);
            free(currentPath);
            currentPath = genNewTopDirPath(dirs);

            dp = readdir(top);
        }
    }

    printe("Could not find the file in any subdirectory. Are you sure this file exists?");
    free(currentPath);

    return NULL;
}