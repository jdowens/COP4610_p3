#include "directoryparse.h"
#include "utility.h"

unsigned int CURRENT_CLUSTER = 0;
unsigned int PREVIOUS_CLUSTER_STACK[256];
unsigned int PREVIOUS_CLUSTER_INDEX = 0; 

struct DirectoryEntry* GetDirectoryContents(unsigned int clusterNum)
{
	FILE* inFile = GetImageFile();

	// maximum entries is 2048
	static struct DirectoryEntry returnArray[2048];
	unsigned int index = 0;

	// raw data read
	char rawData[32];

	// next cluster
	unsigned int nextClusterIndex = clusterNum;

	do
	{
		unsigned int byteIndex = FindFirstSectorOfCluster(nextClusterIndex);
		//printf("Byte index: %x\n", byteIndex);
	
		unsigned int byteIndexOffset = 0;
		do
		{
			fseek(inFile, byteIndex + byteIndexOffset, SEEK_SET);
			fread(rawData, sizeof(char), 32, inFile);
		
			// if first byte 0x00 or 0xE5 (slide 10 of Slides_Week12.ppt)
			// then end of directory
			if (rawData[0] == 0x00 || (unsigned char)rawData[0] == 0xE5)
			{
				byteIndexOffset += 32;
				continue;
			}
			else
			{
				// long name do nothing
				if (rawData[11] & 0x0F)
				{
		
				}
				else
				{
					int i;
					//printf("DIR_Name: ");
					// assign DIR_Name
					for (i = 0; i < 11; i++)
					{
						returnArray[index].DIR_Name[i] = rawData[i];
						//printf("%c", returnArray[index].DIR_Name[i]);
					}
					// null terminate DIR_Name
					returnArray[index].DIR_Name[11] = '\0';
					// assign DIR_Attr
					returnArray[index].DIR_Attr = rawData[11];
					// assign DIR_FstClus
					unsigned char tempArray[4];
					tempArray[0] = rawData[26];
					tempArray[1] = rawData[27];
					tempArray[2] = rawData[20];
					tempArray[3] = rawData[21];
					returnArray[index].DIR_FstClus = little_to_big(tempArray, 4);
					// assign DIR_FileSize
					returnArray[index].DIR_FileSize = little_to_big(rawData+28, 4);
					/*printf("\nDIR_Attr: %x\n", returnArray[index].DIR_Attr);
					printf("DIR_FstClus: %x\n", returnArray[index].DIR_FstClus);
					printf("DIR_ByteOffset: %x\n", FindFirstSectorOfCluster(returnArray[index].DIR_FstClus));
					printf("DIR_FileSize: %x\n", returnArray[index].DIR_FileSize);
					printf("\n");*/
					returnArray[index].DIR_EntryByteAddress = byteIndex+byteIndexOffset;
					returnArray[index++].END_OF_ARRAY = 0;
				}
			}
			byteIndexOffset += 32;
		} while (rawData[0] != 0x00 /*&& rawData[0] != 0xE5*/ && byteIndexOffset < GetBytesPerSec()*GetSecPerClus());
		nextClusterIndex = next_cluster(nextClusterIndex);
	} while (nextClusterIndex < 0x0FFFFFF8);
	returnArray[index].END_OF_ARRAY = 1;
	return returnArray;
}

unsigned int NameToClusterNumber(const char* name)
{
	struct DirectoryEntry* tmp = GetDirectoryContents(GetCurrentDirectoryClusterNum());
	unsigned int index = 0;
	while (!tmp[index].END_OF_ARRAY)
	{
		if (strcmp(tmp[index].DIR_Name, name) == 0)
		{
			return tmp[index].DIR_FstClus;
		}
		index++;
	}
	return 0xFFFFFFFF;
}

unsigned int GetCurrentDirectoryClusterNum()
{
	return CURRENT_CLUSTER;
}

void SetCurrentDirectoryClusterNum(unsigned int clusterNum)
{
	CURRENT_CLUSTER = clusterNum;
}

unsigned int GetPreviousDirectoryClusterNum()
{
	if (PREVIOUS_CLUSTER_INDEX == 0)
		return 0xFFFFFFFF;
	else
		return PREVIOUS_CLUSTER_STACK[PREVIOUS_CLUSTER_INDEX-1];
}

unsigned int PopPreviousDirectoryClusterNum()
{
	if (PREVIOUS_CLUSTER_INDEX == 0)
		return 0xFFFFFFFF;
	else
	{
		PREVIOUS_CLUSTER_INDEX--;
		return PREVIOUS_CLUSTER_STACK[PREVIOUS_CLUSTER_INDEX];
	}
}

void PushPreviousDirectoryClusterNum(unsigned int clusterNum)
{
	PREVIOUS_CLUSTER_STACK[PREVIOUS_CLUSTER_INDEX++] = clusterNum;
}

void WriteDirectoryEntry(struct DirectoryEntry dir, unsigned int clusterNum)
{
	unsigned int byteOffset = FindFirstFreeDirectoryEntry(clusterNum);
	int i;
	for (i = 0; i < 11; i++)
	{
		WriteCharToImage(dir.DIR_Name[i], byteOffset+i);	
	}
	unsigned int DIR_FstClusLO = dir.DIR_FstClus&0x00FF;
	unsigned int DIR_FstClusHI = dir.DIR_FstClus>>16;
	WriteShortToImage(DIR_FstClusLO, byteOffset+26);
	WriteShortToImage(DIR_FstClusHI, byteOffset+20);
	WriteCharToImage(dir.DIR_Attr, byteOffset+11);
	WriteIntToImage(dir.DIR_FileSize, byteOffset+28);
}

void PrintDirectoryVector(const struct DirectoryEntry* d)
{
	int i = 0;
	printf("Directory Vector:\n--------------------------------\n");
	for (i = 0; !d[i].END_OF_ARRAY; i++)
	{
		printf("DIR_Name: %s\n", d[i].DIR_Name);
		printf("DIR_Attr: %x\n", d[i].DIR_Attr);
		printf("DIR_FstClus: %x\n", d[i].DIR_FstClus);
		printf("DIR_FstClusByteAddress: %x\n", FindFirstSectorOfCluster(d[i].DIR_FstClus));
		printf("DIR_FileSize: %x\n", d[i].DIR_FileSize);
		printf("--------------------------------\n");
	}
}

unsigned int next_cluster(unsigned int current_cluster){
	unsigned char temp1[4];
      	FILE* ImageFile= GetImageFile();
	

	//the start of the fat table is 4000
        //CHANGE THE HARD CODED NUMBER
        unsigned int temp = FAT_Start() + current_cluster*4;
        //printf("Temp:%d \n",temp);

	
	fseek(ImageFile,temp,SEEK_SET);
        fread(temp1,sizeof(unsigned char),4,ImageFile);

        return little_to_big(temp1,4);
}

void TestNextCluster(){
	printf("HERE: %x \n",next_cluster(2));
}
