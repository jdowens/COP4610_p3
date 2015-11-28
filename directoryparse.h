#ifndef _DIRECTORYPARSE_H
#define _DIRECTORYPARSE_H

#include "utility.h"

struct DirectoryEntry
{
	char DIR_Name[32];
	unsigned char DIR_Attr;
	unsigned int DIR_FstClus;
	unsigned int DIR_FileSize;
};

struct DirectoryEntry* GetDirectoryContents(unsigned int clusterNum);
struct DirectoryEntry* DirectoryVectorPushBack(struct DirectoryEntry* vec, struct DirectoryEntry* file);

unsigned int next_cluster(unsigned int);
void TestNextCluster();

#endif
