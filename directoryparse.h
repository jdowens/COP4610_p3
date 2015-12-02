#ifndef _DIRECTORYPARSE_H
#define _DIRECTORYPARSE_H

#include "utility.h"

struct DirectoryEntry
{
	char DIR_Name[32];
	unsigned char DIR_Attr;
	unsigned int DIR_FstClus;
	unsigned int DIR_FileSize;
	unsigned char END_OF_ARRAY;
};

struct DirectoryEntry* GetDirectoryContents(unsigned int clusterNum);
struct DirectoryEntry* DirectoryVectorPushBack(struct DirectoryEntry* vec, struct DirectoryEntry* file);
void WriteDirectoryEntry(struct DirectoryEntry dir, unsigned int clusterNum);
void PrintDirectoryVector(const struct DirectoryEntry* d);

// this functions takes the string (in FAT-32 format) of a file, then returns which cluster this file
// starts in (Given that the file exists in the current directory)
// else the function returns 0xFFFFFFFF
unsigned int NameToClusterNumber(const char* name);

unsigned int GetCurrentDirectoryClusterNum();
void SetCurrentDirectoryClusterNum(unsigned int clusterNum);
unsigned int GetPreviousDirectoryClusterNum();
unsigned int PopPreviousDirectoryClusterNum();
void PushPreviousDirectoryClusterNum(unsigned int clusterNum);

unsigned int next_cluster(unsigned int);
void TestNextCluster();

#endif
