#include "size.h"

void size(const char* filename)
{
	struct DirectoryEntry* tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());
	unsigned int index = 0;
	while (!tmp[index].END_OF_ARRAY)
	{
		if (strcmp(tmp[index].DIR_Name, filename) == 0)
		{
			printf("%d\n", tmp[index].DIR_FileSize);
			return;
		}
		index++;
	}
	printf("Error: file does not exist!\n");
}
