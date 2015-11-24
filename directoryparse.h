#ifndef _DIRECTORYPARSE_H
#define _DIRECTORYPARSE_H

#include "utility.h"

struct DirectoryEntry
{
	char name[32];
	unsigned int clusterNum;
	unsigned int clusterOffset;
};

struct DirectoryEntry* GetDirectoryContents(unsigned int clusterNum);
struct DirectoryEntry* DirectoryVectorPushBack(struct DirectoryEntry* vec, struct DirectoryEntry* file);

#endif
