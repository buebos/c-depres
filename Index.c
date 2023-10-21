#include <stdio.h>

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
        printe("Could not find the file in any subdirectory. Are you sure this file exists?");
        return -1;
    }

    struct DepTreeNode *depTreeRoot = DepTreeNode(rootFile);

    fillDepTree(depTreeRoot);

    return 0;
}