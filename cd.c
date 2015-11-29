#include "cd.h"

void cd(const char* path)
{
	struct DirectoryEntry* tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());
	unsigned int index = 0;
	unsigned char somethingFound = 0;
	while (!tmp[index].END_OF_ARRAY)
	{
		if (strcmp(tmp[index].DIR_Name, path) == 0 && tmp[index].DIR_Attr & 0x10)
		{
			SetCurrentDirectoryClusterNum(tmp[index].DIR_FstClus);
			somethingFound = 1;
		}
		++index;
	}
	if (somethingFound == 0)
	{
		printf("Not a valid directory.\n");
	}
}
