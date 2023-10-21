#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/DepTreeNode.h"
#include "src/arrangeFilename.h"
#include "src/searchRelativePath.h"
#include "src/util/printe.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printe("Please provide the name of the file to compile");
        return 1;
    }

    char *filename = arrangeFilename(argv[1]);
    char *rootFile = searchRelativePath(filename);

    if (rootFile == NULL) {
        return 1;
    }

    struct DepTreeNode *depTreeRoot = DepTreeNode(rootFile);
    char *command = calloc(4, sizeof(char));
    strcat(command, "gcc");

    fillDepTree(depTreeRoot, depTreeRoot);

    command = pushDepList(depTreeRoot, command);

    for (int i = 2; i < argc; i++) {
        command = realloc(command, strlen(command) + strlen(argv[i]) + 2);
        strcat(command, " ");
        strcat(command, argv[i]);
    }

    printf("[INFO] Running command: %s\n", command);

    system(command);

    free(command);
    freeDepTree(depTreeRoot);
    free(rootFile);
    free(filename);

    return 0;
}