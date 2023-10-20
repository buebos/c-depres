#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int isDirectory(const char* path) {
    struct stat statbuf;

    if (stat(path, &statbuf) != 0 || path[0] == *".") {
        return 0;
    }

    return S_ISDIR(statbuf.st_mode);
}

/**
 * Searches for the file relative path given the file name
 */
char* searchFileRoute(char* filename) {
    DIR* dir = opendir(".");
    struct dirent* dp;

    if (!dir) {
        return NULL;
    }

    while ((dp = readdir(dir)) != NULL) {
        if (isDirectory(dp->d_name)) {
        }
    }
}