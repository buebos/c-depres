#ifndef __FINDFILEDEPS_H__
#define __FINDFILEDEPS_H__

struct Deps {
    char** items;
    int length;
};

struct Deps* findFileDeps(char* filepath);

#endif