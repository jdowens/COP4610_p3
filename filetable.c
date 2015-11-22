#include "filetable.h"

struct FTEntry* FTRoot = NULL;

void FTAdd(const char* file_name, const char* mode)
{
	struct FTEntry* tmp = calloc(1, sizeof(struct FTEntry));
	strcpy(tmp->name, file_name);
	strcpy(tmp->mode, mode);
	tmp->next = NULL;
	if (FTRoot == NULL)
	{
		FTRoot = tmp;
	}
	else
	{
		struct FTEntry* it = FTRoot;
		while (it->next != NULL)
			it = it->next;
		it->next = tmp;
	}
}

void FTRemove(const char* file_name)
{
	struct FTEntry* it1;
	struct FTEntry* it2 = NULL;
	for (it1 = FTRoot; it1 != NULL; it2 = it1, it1 = it1->next)
	{
		if (strcmp(it1->name, file_name) == 0)
		{
			if (it2 == NULL)
			{
				FTRoot = it1->next;
			}
			else
			{
				it2->next = it1->next;
			}
			free(it1);
			return;
		}
	}
}

int FTIsOpen(const char* file_name)
{
	struct FTEntry* it;
	for (it = FTRoot; it != NULL; it = it->next)
	{
		if (strcmp(it->name, file_name) == 0)
			return 1;
	}
	return 0;
}

int FTIsOpenInRead(const char* file_name)
{
	struct FTEntry* it;
	for (it = FTRoot; it != NULL; it = it->next)
	{
		if (strcmp(it->name, file_name) == 0)
		{
			if (strstr(it->mode, "r") != NULL)
			{
				return 1;
			}
		}
	}
	return 0;
}

int FTIsOpenInWrite(const char* file_name)
{
	struct FTEntry* it;
	for (it = FTRoot; it != NULL; it = it->next)
	{
		if (strcmp(it->name, file_name) == 0)
		{
			if (strstr(it->mode, "w") != NULL)
			{
				return 1;
			}
		}
	}
	return 0;
}

void FTPrint()
{
	printf("\n\nFILE TABLE: \n");	
	if (FTRoot != NULL)
	{
		struct FTEntry* it = FTRoot;
		FTPrintEntry(FTRoot);
		while (it->next != NULL)
		{
			FTPrintEntry(it->next);
			it = it->next;
		}
	}
}

void FTPrintEntry(struct FTEntry* ent)
{
	printf("--------------------------------\n");
	printf("File name: %s\nFile mode: %s\n", ent->name, ent->mode);
	printf("--------------------------------\n");
}

void FTCleanup()
{
	struct FTEntry* it1 = FTRoot;
	struct FTEntry* it2;
	while (it1 != NULL)
	{
		it2 = it1->next;
		free(it1);
		it1 = it2;
	}
	FTRoot = NULL;
}

void TestFileTable()
{
	FTAdd("ROOT_FILE", "r");
	FTAdd("ANOTHER_FILE", "w");
	FTAdd("YET_Another_FiLe", "rw");
	FTPrint();
	printf("ROOT_FILE open?: %i\n", FTIsOpen("ROOT_FILE"));
	printf("ROOT_FILE open in read?: %i\n", FTIsOpenInRead("ROOT_FILE"));
	printf("ROOT_FILE open in write?: %i\n", FTIsOpenInWrite("ROOT_FILE"));
	printf("TEST open?: %i\n", FTIsOpen("TEST"));
	printf("YET_Another_FiLe open in write?: %i\n", FTIsOpenInWrite("YET_Another_FiLe"));
	FTRemove("ROOT_FILE");
	FTPrint();
	FTRemove("YET_Another_FiLe");
	FTPrint();
	FTCleanup();
	FTPrint();
}
