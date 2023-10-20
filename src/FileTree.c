#include <stdlib.h>
#include <string.h>

struct Node {
    char *fileRoute;
    struct Node *next;
    struct Node *children;
};

struct Node *TreeNode(char *fileRoute) {
    struct Node *treeNode = malloc(sizeof(struct Node));

    treeNode->fileRoute = malloc(strlen(fileRoute) + 1);
    strcpy(treeNode->fileRoute, fileRoute);

    treeNode->next = NULL;
    treeNode->children = NULL;

    return treeNode;
}

struct Node *pushChild(struct Node *parent, char *fileRoute) {
    struct Node *newChild = TreeNode(fileRoute);

    if (parent->children == NULL) {
        parent->children = newChild;
    } else {
        struct Node *sibling = parent->children;
        while (sibling->next != NULL) {
            sibling = sibling->next;
        }
        sibling->next = newChild;
    }

    return newChild;
}
