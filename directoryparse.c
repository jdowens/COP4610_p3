#include "directoryparse.h"

struct DirectoryEntry* GetDirectoryContents(unsigned int clusterNum)
{
	FILE* inFile = GetImageFile();

	// maximum entries is 2048
	static struct DirectoryEntry[2048];
	unsigned int index = 0;

	// uses not finished functions
	unsigned int byteIndex = clusterNum*GetBytesPerSector()*GetSectorsPerCluster();

	char rawData[32];

	fseek(inFile, byteIndex, SEEK_SET);
	fread(rawData, sizeof(char), 32, inFile);

	// if first byte 0x00 or 0xE5 (slide 10 of Slides_Week12.ppt)
	// then end of directory
	if (rawData[0] == 0x00 || rawData[0] == 0xE5)
	{

	}
	else
	{
		// long name do nothing
		if (rawData[11] & 0x0F)
		{

		}
		else
		{
			for (int i = 0; i < 11; i++)
			{
				DirectoryEntry[index].name[i] = rawData[i];
			}
			DirectoryEntry[index].clusterNum = 
		}
	}

	// uses not finished functions
	unsigned int nextClusterIndex = GetNextClusterIndex(clusterNum);

	while (nextClusterIndex < 0x0FFFFFF8)
	{
		
	}
}
