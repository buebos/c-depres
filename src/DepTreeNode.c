#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "findFileDeps.h"
#include "util/printe.h"

struct DepTreeNode {
    char *path;
    struct DepTreeNode *firstChild;
    struct DepTreeNode *nextSibling;
};

struct DepTreeNode *DepTreeNode(char *path) {
    struct DepTreeNode *rootNode = calloc(1, sizeof(struct DepTreeNode));

    rootNode->path = path;
    rootNode->firstChild = NULL;
    rootNode->nextSibling = NULL;

    return rootNode;
}

struct DepTreeNode *pushDepTreeFilepath(struct DepTreeNode *parent, char *dep) {
    struct DepTreeNode *node = DepTreeNode(dep);

    if (parent->firstChild == NULL) {
        parent->firstChild = node;
        return parent->firstChild;
    }

    struct DepTreeNode *sibling = parent->firstChild;

    while (sibling->nextSibling != NULL) {
        sibling = sibling->nextSibling;
    }

    sibling->nextSibling = node;

    return sibling->nextSibling;
}

struct DepTreeNode *findDep(struct DepTreeNode *parent, char *filepath) {
    if (parent->firstChild == NULL) {
        return NULL;
    }

    struct DepTreeNode *child = parent->firstChild;

    while (child != NULL) {
        if (strcmp(child->path, filepath) == 0) {
            return child;
        }

        child = findDep(child, filepath);

        if (child) {
            printf("Repeated: %s\n", child->path);
            return child;
        }

        child = child->nextSibling;
    }

    return NULL;
}

void fillDepTree(struct DepTreeNode *parent) {
    struct Deps *deps = findFileDeps(parent->path);

    if (deps == NULL || deps->length == 0) {
        return;
    }

    for (int i = 0; i < deps->length; i++) {
        struct DepTreeNode *child = pushDepTreeFilepath(parent, deps->items[i]);

        printf("%s\t>\t%s\n", parent->path, child->path);
        fillDepTree(child);
    }
}
