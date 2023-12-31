#ifndef __DEPTREENODE_H__
#define __DEPTREENODE_H__

struct DepTreeNode {
    char *path;
    struct DepTreeNode *firstChild;
    struct DepTreeNode *nextSibling;
};

struct DepTreeNode *DepTreeNode(char *path);

struct DepTreeNode *pushDepTreeFilepath(struct DepTreeNode *parent, char *dep);

void fillDepTree(struct DepTreeNode *root, struct DepTreeNode *parent);

char *pushDepList(struct DepTreeNode *parent, char *list);

void freeDepTree(struct DepTreeNode *parent);

#endif