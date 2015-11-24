#include "utility.h"

unsigned int SectorNumber = 0;
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

/*void SetCurrentSectorNum(unsigned int num)
{
	SectorNumber = num;
}

unsigned int GetCurrentSectorNum()
{
	return SectorNumber;
}
*/
void ParseBootSector(void)
{
    unsigned short store_bytes[4];
    char SPC[1];
    fseek(ImageFile, 11, SEEK_SET);
	fread(store_bytes, sizeof(char), 2, ImageFile);		
	BPB_BytesPerSector = store_bytes[0];
	printf("BytesPSec: %i\n", BPB_BytesPerSector);

    fseek(ImageFile, 13, SEEK_SET);
    fread(SPC, sizeof(char), 1, ImageFile);
    BPB_SecPerClus = SPC[0];
    printf("SPC: %i\n", BPB_SecPerClus);

    fseek(ImageFile, 14, SEEK_SET);
    fread(store_bytes, sizeof(char), 2, ImageFile);
    BPB_RsvdSecCnt = store_bytes[0];
    printf("RsvdSecCnt = %i\n", BPB_RsvdSecCnt);

    fseek(ImageFile, 16, SEEK_SET);
    fread(store_bytes, sizeof(char), 1, ImageFile);
    BPB_NumFats = store_bytes[0];
    printf("NumFats = %i\n", BPB_NumFats);

    fseek(ImageFile, 36, SEEK_SET);
    fread(store_bytes, sizeof(char), 4, ImageFile);
    BPB_FATSz32 = store_bytes[0];
    printf("FATSz32 = %i\n", BPB_FATSz32);
    
    // root clus
    fseek(ImageFile, 44, SEEK_SET);
    fread(store_bytes, sizeof(char), 4, ImageFile);
    BPB_RootClus = store_bytes[0];
    printf("RootClus = %i\n", BPB_RootClus);

    FindFirstSectorOfCluster(BPB_RootClus);
}

int FindFirstSectorOfCluster(int N)
{
    int FirstDataSector = BPB_RsvdSecCnt + (BPB_NumFats * BPB_FATSz32);
    int FirstSectorofCluster = ((N - 2) * BPB_SecPerClus) + FirstDataSector;
    int DirLocation = FirstSectorofCluster * BPB_BytesPerSector;
    
    printf("Sector Number of Root Directory: %i\n", DirLocation);
    return DirLocation;
}

void FindFATTable(int N)
{
    int FATOffset = N * 4;

    int ThisFATSecNum =  BPB_RsvdSecCnt + (FATOffset / BPB_BytesPerSector);
    int ThisFATEntOffset = 0;


    //if (N > BPB_RootClus)
    ThisFATEntOffset = FATOffset % BPB_BytesPerSector;   
    
}

// utility function to convert cstring to short(int)
//short StrToShort(char * cstr)
//{
//}

/*
int next_cluster(int current_cluster){
	int temp1=0;
	//the start of the fat table is 4000
	//CHANGE THE HARD CODED NUMBER
	int temp2 = 4000 + current_cluster;
	fseek(ImageFIle,temp2,SEEK_SET);
	fread(temp1,sizeof(char),1,ImageFile);
	return temp1;
}*/

unsigned int little_to_big(char *array, int bytes){
	unsigned int ret = 0;
	int i;
	for(i = 0; i < bytes; i++)
	{
		ret += (unsigned int)(array[i])<<(i*8);
	}
	return ret;
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


