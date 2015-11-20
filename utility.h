#ifndef _UTILITY_H
#define _UTILITY_H
#include <stdio.h>

/////////////
// GLOBALS //
/////////////

// global image file pointer

void OpenImageFile(const char* name);

void ParseBootSector(void);

void FindRootDirectory(void);

#endif
