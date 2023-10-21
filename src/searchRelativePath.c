#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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
        return NULL;
    }

    stack->items[stack->len] = dir;
    stack->len += 1;

    return stack->items[stack->len - 1];
}

DIR* pop(struct DirStack* stack) {
    DIR* topDir = getTop(stack);
    closedir(topDir);
    stack->len -= 1;

    return getTop(stack);
}

char* genNewTopDirPath(struct DirStack* dirs) {
    char* path = calloc(1, sizeof(char));

    for (int i = 0; i < dirs->len; i++) {
        char* dirName = dirs->items[i]->dd_dir.d_name;

        path = realloc(path, strlen(path) + strlen(dirName) + 2);

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
    DIR* top = push(dirs, opendir("."));

    if (top == NULL) {
        return NULL;
    }

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

            top = push(dirs, opendir(currentPath));
        }

        free(dpPath);

        dp = readdir(top);

        if (dp == NULL) {
            top = pop(dirs);

            /**
             * The file was not found even when returning to the
             * root dir
             */
            if (top == NULL) {
                return NULL;
            }

            dp = readdir(top);
        }
    }

    return NULL;
}