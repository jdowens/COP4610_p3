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




