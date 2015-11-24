#ifndef _DIRECTORYPARSE_H
#define _DIRECTORYPARSE_H

struct DirectoryEntry
{
	char name[32];
	unsigned int clusterNum;
	unsigned int clusterOffset;
};



#endif
