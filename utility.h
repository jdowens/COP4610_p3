#ifndef _UTILITY_H
#define _UTILITY_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void OpenImageFile(const char* name);
void CloseImageFile();
FILE* GetImageFile();

void SetCurrentSectorNum(unsigned int num);
int GetCurrentSectorNum();

short GetBytesPerSec(void);
short GetSecPerClus(void);

int FindFirstSectorOfCluster(int N);

void ParseBootSector(void);

void FindRootDirectory(void);

unsigned int little_to_big(unsigned char*, int);

#endif
