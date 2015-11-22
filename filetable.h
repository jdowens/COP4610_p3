#ifndef _FILETABLE_H
#define _FILETABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct FTEntry
{
	char name[32];
	char mode[8];
	struct FTEntry* next;
};

void FTAdd(const char* file_name, const char* mode);

void FTRemove(const char* file_name);

int FTIsOpen(const char* file_name);

int FTIsOpenInRead(const char* file_name);

int FTIsOpenInWrite(const char* file_name);

void FTPrint();

void FTPrintEntry(struct FTEntry* ent);

void FTCleanup();

void TestFileTable();

#endif
