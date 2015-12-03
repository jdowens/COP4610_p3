#include "create.h"

void create(const char* name)
{
	struct DirectoryEntry* tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());
	
	int i;
	for (i = 0; !tmp[i].END_OF_ARRAY; i++)
	{
		if (strcmp(tmp[i].DIR_Name, name) == 0)
		{
			printf("File already exists!\n");
			return;
		}
	}

	struct DirectoryEntry ent;
	strcpy(ent.DIR_Name, name);
	ent.DIR_Attr = 0x00;
	unsigned int firstCluster = FindNextFreeCluster();
	WriteToFAT(firstCluster, 0x0FFFFFF8);
	ent.DIR_FstClus = firstCluster;
	ent.DIR_FileSize = 0x0000;
	WriteDirectoryEntry(ent, GetCurrentDirectoryClusterNum());
}
