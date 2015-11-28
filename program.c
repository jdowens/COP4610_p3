#include "program.h"

char USER_INPUT_RAW[USER_INPUT_BUFFER_LENGTH*5];
char USER_INPUT[5][USER_INPUT_BUFFER_LENGTH];

void RunProgram(void)
{
	int running = 1;
	while (running)
	{
		PrintPrompt();
		GetUserInput();
		if (strcmp(USER_INPUT[0], "exit") == 0)
		{
			running = 0;
		}
		else if (strcmp(USER_INPUT[0], "ls") == 0)
		{
			if (strcmp(USER_INPUT[1], ".") == 0)
				list(GetCurrentDirectoryClusterNum());
			else
			{
				struct DirectoryEntry* tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());
				unsigned int index = 0;
				unsigned int somethingFound = 0;
				while (!tmp[index].END_OF_ARRAY)
				{
					if (strcmp(USER_INPUT[1], tmp[index].DIR_Name) == 0 &&
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
	}
}

void PrintPrompt(void)
{
	printf("=> ");
}

void GetUserInput(void)
{
	char* tmp_buff;
	gets(USER_INPUT_RAW);
	tmp_buff = strtok(USER_INPUT_RAW, " \t\n\0");
	unsigned int index = 0;
	while (tmp_buff != NULL)
	{
		strncpy(USER_INPUT[index], tmp_buff, USER_INPUT_BUFFER_LENGTH);
		printf("%s\n", USER_INPUT[index]);
		tmp_buff = strtok(NULL, " \t\n\0");
		++index;
	}
	int i;
	for (i = index; i < 5; i++)
	{
		memset(USER_INPUT[i], " ", USER_INPUT_BUFFER_LENGTH);
	}
}
