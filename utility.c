#include "utility.h"

FILE* ImageFile = NULL;

const int ENDOFCLUSTER = 268435448;

// boot sector
short BPB_BytesPerSector;
short BPB_SecPerClus;
short BPB_RsvdSecCnt;
short BPB_NumFats;
short BPB_FATSz32;

// root directory
short BPB_RootClus;
unsigned int FAT_StartLoc, FAT_EndLoc;

void OpenImageFile(const char* name)
{
	ImageFile = fopen(name, "r+b");
	if (ImageFile == NULL)
	{
		printf("Image file could not be opened...\n");
		exit(-1);
	}	
}

void CloseImageFile()
{
	if (ImageFile != NULL)
		fclose(ImageFile);
}

FILE* GetImageFile()
{
	return ImageFile;
}

short GetBytesPerSec(void)
{
    return BPB_BytesPerSector;
}

short GetSecPerClus(void)
{
    return BPB_SecPerClus;
}

unsigned int GetRootClusterNum(void)
{
	return BPB_RootClus;
}

/*void SetCurrentSectorNum(unsigned int num)
{
	SectorNumber = num;
}

int GetCurrentSectorNum()
{
	return SectorNumber;
}*/

void ParseBootSector(void)
{
    unsigned short store_bytes[4];
    unsigned char SPC[1];
    int FATOffset, ThisFATSecNum;

    fseek(ImageFile, 11, SEEK_SET);
	fread(store_bytes, sizeof(char), 2, ImageFile);		
	BPB_BytesPerSector = store_bytes[0];
	//printf("BytesPSec: %i\n", BPB_BytesPerSector);

    fseek(ImageFile, 13, SEEK_SET);
    fread(SPC, sizeof(char), 1, ImageFile);
    BPB_SecPerClus = little_to_big(SPC, 1);
    //printf("SPC: %i\n", BPB_SecPerClus);

    fseek(ImageFile, 14, SEEK_SET);
    fread(store_bytes, sizeof(char), 2, ImageFile);
    BPB_RsvdSecCnt = little_to_big(store_bytes, 2);
    //printf("RsvdSecCnt = %i\n", BPB_RsvdSecCnt);

    fseek(ImageFile, 16, SEEK_SET);
    fread(store_bytes, sizeof(char), 1, ImageFile);
    BPB_NumFats = little_to_big(store_bytes, 1);
    //printf("NumFats = %i\n", BPB_NumFats);

    fseek(ImageFile, 36, SEEK_SET);
    fread(store_bytes, sizeof(char), 4, ImageFile);
    BPB_FATSz32 = little_to_big(store_bytes, 4);
    //printf("FATSz32 = %i\n", BPB_FATSz32);
    
    // root clus
    fseek(ImageFile, 44, SEEK_SET);
    fread(store_bytes, sizeof(char), 4, ImageFile);
    BPB_RootClus = little_to_big(store_bytes, 4);
    //printf("RootClus = %i\n", BPB_RootClus);

    FindFirstSectorOfCluster(BPB_RootClus);
    

    // calculate the starting location of the FAT table
    FATOffset = BPB_RootClus * 4;
    ThisFATSecNum =  BPB_RsvdSecCnt + (FATOffset / BPB_BytesPerSector);
    
    FAT_StartLoc = ThisFATSecNum*BPB_BytesPerSector;
    FAT_EndLoc = BPB_FATSz32 * BPB_BytesPerSector + FAT_StartLoc;
}

int FindFirstSectorOfCluster(int N)
{
    int FirstDataSector = BPB_RsvdSecCnt + (BPB_NumFats * BPB_FATSz32);
    int FirstSectorofCluster = ((N - 2) * BPB_SecPerClus) + FirstDataSector;
    int DirLocation = FirstSectorofCluster * BPB_BytesPerSector;
//  printf("Byte address of Current Cluster: %x\n", DirLocation);
    return DirLocation;
}

unsigned int little_to_big(unsigned char *array, int bytes){
	unsigned int ret = 0;
	int i;
	for(i = 0; i < bytes; i++)
	{
		ret += (unsigned int)(array[i])<<(i*8);
	}
	return ret;
}

unsigned char* big_to_little(unsigned int value, unsigned int size)
{
	static unsigned char ret[4];
	//printf("%016llX\n", ret);
	memset(ret, 0, 4);
	unsigned int i = 0;
	unsigned int mask = 0x000000FF;
	unsigned int tmp_val = value;
	for (i = 0; i < size; i++)
	{
		ret[i] = tmp_val&mask;
		tmp_val = tmp_val >> 8;
	}
	return ret;
}

void WriteIntToImage(unsigned int value, unsigned int offset)
{
	fseek(ImageFile, offset, SEEK_SET);
	unsigned char* tmp = big_to_little(value, 4);
	for (int i = 0; i < 4; i++)
	{
		fputc(tmp[i], ImageFile);
	}
}

void WriteShortToImage(unsigned short value, unsigned int offset)
{
	fseek(ImageFile, offset, SEEK_SET);
	unsigned char* tmp = big_to_little(value, 2);
	for (int i = 0; i < 2; i++)
	{
		fputc(tmp[i], ImageFile);
	}
}

void WriteCharToImage(unsigned char value, unsigned int offset)
{
	fseek(ImageFile, offset, SEEK_SET);
	fputc(value, ImageFile);
}

unsigned int FindNextFreeCluster(void)
{
    unsigned int cluster_value;
    unsigned short store_bytes[4];
    unsigned int seek_pos = FAT_StartLoc;
    unsigned int cluster_number = 0;
    
    do
    {
        fseek(ImageFile, seek_pos, SEEK_SET);
        fread(store_bytes, sizeof(char), 4, ImageFile);
        cluster_value = little_to_big(store_bytes, 4);
        seek_pos += 4;
        ++cluster_number;
    }
    while(cluster_value != 0x00000000 || seek_pos >= FAT_EndLoc);

    if(seek_pos == FAT_EndLoc)
    {
        printf("ERROR: No available clusters, image is FULL.\n");
	return 0xFFFFFFFF;
    }

    cluster_number--;
    //printf("The next free cluster is at: %i\n", cluster_number);
    //printf("The byte address is:%x\n", FindFirstSectorOfCluster(cluster_number));

    return cluster_number;
}

void WriteToFAT(unsigned int cluster_number, unsigned int next_cluster)
{
	unsigned int byteOffsetTable1 = FAT_Start()+cluster_number*4;
	unsigned int byteOffsetTable2 = FAT_End()+cluster_number*4;
	WriteIntToImage(next_cluster, byteOffsetTable1);
	WriteIntToImage(next_cluster, byteOffsetTable2);
}

unsigned int FindFirstFreeDirectoryEntry(unsigned int clusterNum)
{
	unsigned int currentClusterIndex = clusterNum;
	unsigned int previousClusterIndex = clusterNum;
	do
	{
		unsigned int byteStart = FindFirstSectorOfCluster(currentClusterIndex);
		unsigned int byteOffset = 0;
		char rawData[1];
		do
		{
			fseek(ImageFile, byteStart + byteOffset, SEEK_SET);
			fread(rawData, sizeof(char), 1, ImageFile);
			byteOffset += 32;
		} while (*rawData != 0xE5 && *rawData != 0x00 && byteOffset < GetBytesPerSec()*GetSecPerClus());
		if (*rawData == 0xE5 || *rawData == 0x00)
		{
			return byteStart+byteOffset-32;
		}
		previousClusterIndex = currentClusterIndex;
		currentClusterIndex = next_cluster(currentClusterIndex);
	} while (currentClusterIndex < 0x0FFFFFF8);
	unsigned int allocatedCluster = FindNextFreeCluster();
	WriteToFAT(previousClusterIndex, allocatedCluster);
	WriteToFAT(allocatedCluster, 0x0FFFFFF8);
	return FindFirstSectorOfCluster(allocatedCluster);
}

unsigned int FAT_Start(void)
{
    return FAT_StartLoc;
}

unsigned int FAT_End(void)
{
    return FAT_EndLoc;
}
// NOTES:
// 
// FATOffset = N * 4
// e.g for root N = RootClus where RootClus = 2, so FATOffset = 8
//
// ThisFATSecNum = BPB_RsvdSecCnt + (FATOffset / BPB_BytesPerSec)
// ThisFATEntOffset = REM(FATOffset / BPB_BytesPerSec)
// 
// For RootDirectory we calculate ThisFATSecNum to be 32. 32 * BytesPerSector = 16384 = 4000 in hex
// So FAT table starts at 4000.
//


