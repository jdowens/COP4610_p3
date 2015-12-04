#include "rm.h"

const unsigned int EMPTY_BYTE = 0xE5;

void rm (char * fname)
{
    unsigned int firstClusterNum;
    unsigned int dirAddress;
    
    struct DirectoryEntry* tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());

    int i;
    int found = 0;
    for (i = 0; !tmp[i].END_OF_ARRAY; i++)
    {
	if (strcmp(tmp[i].DIR_Name, fname) == 0)
	{
		if (tmp[i].DIR_Attr&0x10)
		{
			printf("Cannot use rm on a directory\n");
			return;
		}
		else
		{
			found = 1;
			break;
		}
	}
    }
    if (found == 0)
    {
	printf("File does not exist\n");
	return;
    }

    WriteCharToImage(EMPTY_BYTE, tmp[i].DIR_EntryByteAddress);
    firstClusterNum = tmp[i].DIR_FstClus;
    RemoveFromFAT(firstClusterNum);
    //printf("firstFClusterNum %i\n", firstClusterNum); 
}

void rmdir (char * dirname)
{
    unsigned int firstClusterNum;
    unsigned int dirAddress;
    
    struct DirectoryEntry* tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());

    int i;
    int found = 0;
    for (i = 0; !tmp[i].END_OF_ARRAY; i++)
    {
	if (strcmp(tmp[i].DIR_Name, dirname) == 0)
	{
		if (!tmp[i].DIR_Attr&0x10)
		{
			printf("Cannot use rmdir on a file\n");
			return;
		}
		else
		{
			found = 1;
			break;
		}
	}
    }
    if (found == 0)
    {
	printf("Directory does not exist\n");
	return;
    }

    cd(dirname);
    tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());
    int j;
    for (j = 0; !tmp[j].END_OF_ARRAY; j++)
    {
    }
    if (j > 2)
    {
	printf("Directory not empty\n");
	cd("..");
	return;
    }
    cd("..");
    tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());

    WriteCharToImage(EMPTY_BYTE, tmp[i].DIR_EntryByteAddress);
    firstClusterNum = tmp[i].DIR_FstClus;
    RemoveFromFAT(firstClusterNum);
    //printf("firstFClusterNum %i\n", firstClusterNum); 
}

void RemoveFromFAT(unsigned int cluster_number)
{
   // might need dynamic array here
   unsigned int clusterNums[65536];
   unsigned short index = 0;
   unsigned int temp = cluster_number;

   do
   {
       // add cluster numbers to stack
       clusterNums[index] = temp;
       temp = next_cluster(temp);
       index++;
   }
   while(temp < 0x0FFFFFF8);

   // pop cluster numbers and set to 0
   for(int i = index-1; i >= 0; i--)
   {
       ZeroDataEntry(clusterNums[i]);
       WriteToFAT(clusterNums[i], 0);
   }
}

void ZeroDataEntry(unsigned int clusterNum)
{
	FILE* ImageFile = GetImageFile();
	unsigned int byteAddress = FindFirstSectorOfCluster(clusterNum);
	unsigned int numBytes = GetBytesPerSec()*GetSecPerClus();
	fseek(ImageFile, byteAddress, SEEK_SET);
	unsigned int i;
	for (i = 0; i < numBytes; i++)
	{
		fputc('\0', ImageFile);
	}
}
