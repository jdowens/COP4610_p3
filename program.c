#include "program.h"
#include "ls.h"

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
			if (strcmp(USER_INPUT[1], ". . . . .") == 0)
			{
				//printf("Requires an argument for path name\n");
				list(GetCurrentDirectoryClusterNum());
			}
			else
			{
				ls(USER_INPUT[1]);
			}
		}
		else if (strcmp(USER_INPUT[0], "cd") == 0)
		{
			if (strcmp(USER_INPUT[1], ". . . . .") == 0)
			{
				printf("Requires an argument for path name\n");
			}
			else if (strcmp(USER_INPUT[1], ".") == 0)
			{
				// do nothing
			}
			else
			{
				//char parsed_dir[USER_INPUT_BUFFER_LENGTH];
				//strcpy(parsed_dir, USER_INPUT[1]);
				//ToFAT32(parsed_dir);
				//printf("!%s!\n", parsed_dir);
				cd(USER_INPUT[1]);
			}
		}
		else if (strcmp(USER_INPUT[0], "size") == 0)
		{
			if (strcmp(USER_INPUT[1], ". . . . .") == 0)
			{
				printf("Requires a file name argument\n");
			}
			else
			{
				size(USER_INPUT[1]);
			}
		}
		else if (strcmp(USER_INPUT[0], "open") == 0)
		{
			if (strcmp(USER_INPUT[1], ". . . . .") == 0 ||
			    strcmp(USER_INPUT[2], ". . . . .") == 0)
			{
				printf("Requires a file name argument followed by a mode argument\n");
			}
			else
			{
				char parsed_dir[USER_INPUT_BUFFER_LENGTH];
				strcpy(parsed_dir, USER_INPUT[1]);
				ToFAT32(parsed_dir);
				open(parsed_dir, USER_INPUT[2]);
			}
		}
		else if (strcmp(USER_INPUT[0], "close") == 0)
		{
			if (strcmp(USER_INPUT[1], ". . . . .") == 0)
			{
				printf("Requires a file name argument\n");
			}
			else
			{
				char parsed_dir[USER_INPUT_BUFFER_LENGTH];
				strcpy(parsed_dir, USER_INPUT[1]);
				ToFAT32(parsed_dir);
				close(parsed_dir);
			}
		}
		else if (strcmp(USER_INPUT[0], "read") == 0)
		{
			if (strcmp(USER_INPUT[1],". . . . .") == 0 ||
			    strcmp(USER_INPUT[2],". . . . .") == 0 ||
			    strcmp(USER_INPUT[3],". . . . .") == 0)
			{
				printf("Requires arguments for filename, position, and size\n");
			}
			else
			{
				char parsed_dir[USER_INPUT_BUFFER_LENGTH];
				strcpy(parsed_dir, USER_INPUT[1]);
				ToFAT32(parsed_dir);
				read(parsed_dir, atoi(USER_INPUT[2]), atoi(USER_INPUT[3]));
			}
		}
		else if (strcmp(USER_INPUT[0], "debug") == 0)
		{
			printf("CURRENT_CLUSTER: %d\n", GetCurrentDirectoryClusterNum());
			printf("CURRENT_CLUSTER BYTE_ADDRESS: 0x%x\n", FindFirstSectorOfCluster(GetCurrentDirectoryClusterNum()));
			FTPrint();
			PrintDirectoryVector(GetDirectoryContents(GetCurrentDirectoryClusterNum()));
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
		strcpy(USER_INPUT[i], ". . . . .");
		//memset(USER_INPUT[i], " ", USER_INPUT_BUFFER_LENGTH);
	}
}
