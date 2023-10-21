#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parseFileDeps.h"
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
    struct DepTreeNode *dep;

    if (parent->firstChild != NULL) {
        dep = findDep(parent->firstChild, filepath);

        if (dep != NULL) {
            return dep;
        }
    }

    dep = parent->nextSibling;

    while (dep != NULL) {
        if (strcmp(dep->path, filepath) == 0) {
            return dep;
        }

        dep = dep->nextSibling;
    }

    return NULL;
}

void fillDepTree(struct DepTreeNode *root, struct DepTreeNode *parent) {
    struct Deps *deps = parseFileDeps(parent->path);

    if (deps == NULL || deps->length == 0) {
        return;
    }

    for (int i = 0; i < deps->length; i++) {
        struct DepTreeNode *dep = findDep(root, deps->items[i]);

        if (dep != NULL) {
            // printf("%s\t>\t__%s__\n", parent->path, dep->path);

            continue;
        }

        dep = pushDepTreeFilepath(parent, deps->items[i]);

        // printf("%s\t>\t%s\n", parent->path, dep->path);

        fillDepTree(root, dep);
    }
}

char *pushDepList(struct DepTreeNode *parent, char *list) {
    if (parent->firstChild != NULL) {
        list = pushDepList(parent->firstChild, list);
    }

    struct DepTreeNode *dep = parent;

    while (dep != NULL) {
        list = realloc(list, strlen(list) + strlen(dep->path) + 2);

        strcat(list, " ");
        strcat(list, dep->path);

        dep = dep->nextSibling;
    }

    return list;
}

void freeDepTree(struct DepTreeNode *parent) {
    if (parent->firstChild != NULL) {
        freeDepTree(parent->firstChild);
    }

    struct DepTreeNode *dep = parent;
    struct DepTreeNode *nextPtr = dep->nextSibling;

    while (dep != NULL) {
        nextPtr = dep->nextSibling;

        free(dep->path);
        free(dep);

        dep = nextPtr;
    }

    free(nextPtr);
}
