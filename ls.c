#include "ls.h"

void list(unsigned int clusterNum)
{
	struct DirectoryEntry* tmp = GetDirectoryContents(clusterNum);
	unsigned int index = 0;
	while (!tmp[index].END_OF_ARRAY)
	{
		printf("%s ", tmp[index++].DIR_Name);
	}
	printf("\n");
}

// works on non parsed strings (non FAT32 format)
void ls(const char* path)
{
	if (strcmp(path, ".") == 0)
		list(GetCurrentDirectoryClusterNum());
	else if (strcmp(path, "..") == 0)
	{
		unsigned int prevClus = GetPreviousDirectoryClusterNum();
		if (prevClus != 0xFFFFFFFF)
			list(prevClus);
	}
	else
	{
		struct DirectoryEntry* tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());
		unsigned int index = 0;
		unsigned int somethingFound = 0;
		char parsed_dir[USER_INPUT_BUFFER_LENGTH];
		strcpy(parsed_dir, path);
		ToFAT32(parsed_dir);
		while (!tmp[index].END_OF_ARRAY)
		{
			if (strcmp(parsed_dir, tmp[index].DIR_Name) == 0 &&
			    tmp[index].DIR_Attr & 0x10)
			{
				list(tmp[index].DIR_FstClus);
				somethingFound = 1;
			}
			++index;
		}
		if (!somethingFound)
		{
			printf("Directory not found...\n");
		}
	}
}


