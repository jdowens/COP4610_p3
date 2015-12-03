#include "program.h"

//char USER_INPUT_RAW[USER_INPUT_BUFFER_LENGTH*5];
char* USER_INPUT[5];
//char* USER_INPUT_0[USER_INPUT_BUFFER_LENGTH];
//char* USER_INPUT_1[USER_INPUT_BUFFER_LENGTH];
//char* USER_INPUT_2[USER_INPUT_BUFFER_LENGTH];
//char* USER_INPUT_3[USER_INPUT_BUFFER_LENGTH];

void RunProgram(void)
{
	int running = 1;
	//USER_INPUT[0] = USER_INPUT_0;
	//USER_INPUT[1] = USER_INPUT_1;
	//USER_INPUT[2] = USER_INPUT_2;
	//USER_INPUT[3] = USER_INPUT_3;
	//USER_INPUT[4] = NULL;

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
		else if (strcmp(USER_INPUT[0], "test") == 0)
		{
			struct DirectoryEntry ent;
			char parsed_dir[USER_INPUT_BUFFER_LENGTH];
			strcpy(parsed_dir, USER_INPUT[1]);
			ToFAT32(parsed_dir);
			strcpy(ent.DIR_Name, parsed_dir);
			ent.DIR_Attr = 0x20;
			ent.DIR_FstClus = 0x1234;
			ent.DIR_FileSize = 0x0000;
			WriteDirectoryEntry(ent, GetCurrentDirectoryClusterNum());
		}
		else if (strcmp(USER_INPUT[0], "debug") == 0)
		{
			printf("CURRENT_CLUSTER: %d\n", GetCurrentDirectoryClusterNum());
			printf("CURRENT_CLUSTER BYTE_ADDRESS: 0x%x\n", FindFirstSectorOfCluster(GetCurrentDirectoryClusterNum()));
			FTPrint();
			PrintDirectoryVector(GetDirectoryContents(GetCurrentDirectoryClusterNum()));
		}
	}
	int k;
	for (k = 0; k < 5; k++)
	{
		free(USER_INPUT[k]);
	}
}

void PrintPrompt(void)
{
	printf("=> ");
}

void GetUserInput(void)
{
	char tmp;
	int j;
	for (j = 0; j < 5; j++)
	{
		if (USER_INPUT[j] != NULL)
			free(USER_INPUT[j]);
		USER_INPUT[j] = (char*)calloc(1, sizeof(char));
		USER_INPUT[j][0] = '\0';
	}
	unsigned int user_vector_index = 0;
	do
	{
		tmp = fgetc(stdin);
		if (user_vector_index == 4)
		{
			if (tmp != '\"')
				printf("Expected quotes around final argument");
			tmp = fgetc(stdin);
			while (tmp != '\"')
			{
				USER_INPUT[user_vector_index] = DynStrPushBack(USER_INPUT[user_vector_index], tmp);
				tmp = fgetc(stdin);
			}
			// get the rest of the stream
			while (tmp != '\n' && tmp != '\0')
			{
				tmp = fgetc(stdin);
			}
		}
		else
		{
			while(tmp != ' ' && tmp != '\n' && tmp != '\0')
			{
				USER_INPUT[user_vector_index] = DynStrPushBack(USER_INPUT[user_vector_index], tmp);
				tmp = fgetc(stdin);
			}
		}
		printf("%s\n", USER_INPUT[user_vector_index]);
		user_vector_index++;
	}while (tmp != '\n' && tmp != '\0' && user_vector_index < 5);

	int i;
	for (i = user_vector_index; i < 5; i++)
	{
		free(USER_INPUT[i]);
		USER_INPUT[i] = calloc(10, sizeof(char));
		strcpy(USER_INPUT[i], ". . . . .");
		//memset(USER_INPUT[i], " ", USER_INPUT_BUFFER_LENGTH);
	}
}
/*
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
}*/
