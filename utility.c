#include "utility.h"


FILE* ImageFile = NULL;

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
}

void FindRootDirectory(void)
{
    short FirstDataSector = BPB_RsvdSecCnt + (BPB_NumFats * BPB_FATSz32);

    short FirstSectorofCluster = ((BPB_RootClus - 2) * BPB_SecPerClus) + FirstDataSector;

    printf("Sector Number of Root Directory: %i\n", FirstSectorofCluster);
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
	unsigned short array[4];
	unsigned int ret = 0;
	for(int i = 0; i < bytes; i++)
	{
		ret += array[i]<<(i*8);
	}
	return ret;
}
