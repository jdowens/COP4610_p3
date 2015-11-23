#ifndef _UTILITY_H
#define _UTILITY_H
#include <stdio.h>
#include <stdlib.h>

void OpenImageFile(const char* name);
void CloseImageFile();
FILE* GetImageFile();

void SetCurrentSectorNum(unsigned int num);
int GetCurrentSectorNum();

void ParseBootSector(void);

void FindRootDirectory(void);

#endif
