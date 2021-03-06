#include "cd.h"

void cd(const char* path)
{
	if (strcmp(path, "..") == 0)
	{
		unsigned int previousClus = PopPreviousDirectoryClusterNum();
		if (previousClus == 0xFFFFFFFF)
		{

		}
		else
		{
			SetCurrentDirectoryClusterNum(previousClus);
		}
	}
	else
	{
		char parsed_dir[USER_INPUT_BUFFER_LENGTH];
		strcpy(parsed_dir, path);
		ToFAT32(parsed_dir);
		struct DirectoryEntry* tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());
		unsigned int index = 0;
		unsigned char somethingFound = 0;
		while (!tmp[index].END_OF_ARRAY)
		{
			if (strcmp(tmp[index].DIR_Name, parsed_dir) == 0 && tmp[index].DIR_Attr & 0x10)
			{
				PushPreviousDirectoryClusterNum(GetCurrentDirectoryClusterNum());
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
}
