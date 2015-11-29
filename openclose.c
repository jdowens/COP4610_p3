#include "openclose.h"

void open(const char* file_name, const char* mode)
{
	if (strcmp(mode, "r") != 0 && strcmp(mode, "w") != 0 &&
	    strcmp(mode, "rw") != 0 && strcmp(mode, "wr") != 0)
	{
		printf("Invalid mode, options are: r w rw wr\n");
	}
	else if (FTIsOpen(file_name))
	{
		printf("File already open.\n");
	}
	else
	{
		struct DirectoryEntry* tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());
		unsigned int index = 0;
		while (!tmp[index].END_OF_ARRAY)
		{
			if (tmp[index].DIR_Attr & 0x10)
			{
				printf("Cannot open a directory\n");
				return;
			}
			if (strcmp(file_name, tmp[index].DIR_Name) == 0)
			{
				FTAdd(file_name, mode);
				return;
			}
			++index;
		}	
		printf("File not found in current directory.\n");
	}
}

void close(const char* file_name)
{
	if (FTIsOpen(file_name))
	{
		FTRemove(file_name);
	}
	else
	{
		printf("File not open.\n");
	}
}
