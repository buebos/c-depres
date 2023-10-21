#ifndef __parseFileDeps_H__
#define __parseFileDeps_H__

struct Deps {
    char** items;
    int length;
};

struct Deps* parseFileDeps(char* filepath);

#endif