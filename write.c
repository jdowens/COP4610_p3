#include "write.h"

const char* SPACE=" ";

void write(const char* FILE_NAME, int POSITION, int NUM_BYTES, const char* STRING){
	struct DirectoryEntry* directoryArray = GetDirectoryContents(GetCurrentDirectoryClusterNum());
	int index = 0;
	int found = 0;
	FILE* ImageFile = GetImageFile();
	for (index = 0; !directoryArray[index].END_OF_ARRAY; index++)
	{
		if (strcmp(FILE_NAME, directoryArray[index].DIR_Name) == 0)
		{
			if (directoryArray[index].DIR_Attr & 0x10)
			{
				printf("Cannot write to directory!\n");
				return;
			}
			found = 1;
			break;
		}
	}

	if (!found)
	{
		printf("File not found!\n");
		return;
	}

	unsigned int clusterNum = directoryArray[index].DIR_FstClus;
	unsigned int previousClusterNum = clusterNum;
	int numSpaces = POSITION - directoryArray[index].DIR_FileSize;
	if (numSpaces < 0)
		numSpaces = 0;

	update_filesize(directoryArray[index].DIR_EntryByteAddress,
			directoryArray,
			index,
			NUM_BYTES,
			POSITION,
			0);
	
	unsigned int bytesToWrite = NUM_BYTES + numSpaces;
	unsigned int bytes_per_clus = GetSecPerClus()*GetBytesPerSec();
	unsigned int end_position = POSITION + NUM_BYTES - 1;

	// allocate needed clusters
	while (end_position >= bytes_per_clus)
	{
		previousClusterNum = clusterNum;
		clusterNum = next_cluster(clusterNum);
		end_position -= bytes_per_clus;
		if (clusterNum >= 0x0FFFFFF8)
		{
			add_cluster(previousClusterNum);
			clusterNum = next_cluster(previousClusterNum);
		}
	}

	// find starting byteAddress
	clusterNum = directoryArray[index].DIR_FstClus;
	unsigned int start_position = (numSpaces > 0) ? directoryArray[index].DIR_FileSize : POSITION;
	while (start_position >= bytes_per_clus)
	{
		clusterNum = next_cluster(clusterNum);
		start_position -= bytes_per_clus;
	}

	unsigned int byteAddress = FindFirstSectorOfCluster(clusterNum)+start_position;
	fseek(ImageFile, byteAddress, SEEK_SET);
	unsigned int strIt = 0;
	unsigned int clusterIt = start_position;

	while(bytesToWrite)
	{
		if (numSpaces)
		{
			fputc(' ', ImageFile);
			numSpaces--;
		}
		else
		{
			fputc(STRING[strIt], ImageFile);
			strIt++;
		}
		clusterIt++;
		if (clusterIt >= bytes_per_clus)
		{
			clusterNum = next_cluster(clusterNum);
			byteAddress = FindFirstSectorOfCluster(clusterNum);
			fseek(ImageFile, byteAddress, SEEK_SET);
			clusterIt = 0;
		}
		bytesToWrite--;
	}
}//end write

void add_cluster(int cluster_number){
	int next_free_cluster = FindNextFreeCluster();//find next free cluster
	WriteToFAT(cluster_number,next_free_cluster);//update FAT
	WriteToFAT(next_free_cluster,0x0ffffff8);//update FAT
}

void write_at_position(int string_position,int POSITION,int written_bytes, int cluster_number,const char* STRING){
	FILE* ImageFile = GetImageFile();
	fseek(ImageFile,FindFirstSectorOfCluster(cluster_number)+POSITION,SEEK_SET);
	fwrite(&STRING[string_position],sizeof(char),written_bytes,ImageFile);
}

int error_check(const char* FILE_NAME,struct DirectoryEntry* directoryArray,int POSITION,int NUM_BYTES,const char* STRING){
/*      if(FTIsOpen(FILE_NAME) == 0){
                printf("Error: File not open.");
                return;
        }//end if

      if(FTIsOpenInWrite(FILE_NAME) == 0){
                printf("Error: File not in write mode.");
                return;
        }//end if
*/
        int index=0;
        for(index = 0; !directoryArray[index].END_OF_ARRAY; index++){
                if( strcmp( FILE_NAME,directoryArray[index].DIR_Name ) == 0 ){
                        printf("%s %d %d %s \n",directoryArray[index].DIR_Name,POSITION,NUM_BYTES,STRING);
                        break;
                }//end if
        }//end for
        if(directoryArray[index].END_OF_ARRAY){
                        printf("Error: Did not find the file name.\n");
                        return;
        }//end if

        //check it is not a directory
        if (strcmp(directoryArray[index].DIR_Name, FILE_NAME) == 0 && directoryArray[index].DIR_Attr & 0x10){
                printf("Error: This is a directory.");
                return;
        }//end if

	return index;
}

void update_filesize(unsigned int address,struct DirectoryEntry* directoryArray, int index, int NUM_BYTES, int POSITION, int num_spaces){
        /*NUM_BYTES += num_spaces;//add the number of spaces to the total
	if( NUM_BYTES >= directoryArray[index].DIR_FileSize && POSITION < directoryArray[index].DIR_FileSize){
                directoryArray[index].DIR_FileSize = NUM_BYTES;//add to file size
        }
        if( (NUM_BYTES >= directoryArray[index].DIR_FileSize && POSITION > directoryArray[index].DIR_FileSize)
           || (NUM_BYTES < directoryArray[index].DIR_FileSize && POSITION > directoryArray[index].DIR_FileSize) ){
                directoryArray[index].DIR_FileSize += NUM_BYTES;
        }
        if(NUM_BYTES < directoryArray[index].DIR_FileSize && POSITION < directoryArray[index].DIR_FileSize){}
        if(POSITION ==  directoryArray[index].DIR_FileSize){
                directoryArray[index].DIR_FileSize = NUM_BYTES-1;
        }*/
	
	// new code
	unsigned int newSize = directoryArray[index].DIR_FileSize;
	unsigned int currentSize = newSize;
	if (POSITION >= currentSize)
		newSize = POSITION + NUM_BYTES;
	else if (POSITION+NUM_BYTES > currentSize)
		newSize += POSITION+NUM_BYTES - currentSize;
	 //WriteIntToImage(directoryArray[index].DIR_FileSize,address+28);
	 //printf("New File Size: %s %d\n",directoryArray[index].DIR_Name,directoryArray[index].DIR_FileSize);
	 WriteIntToImage(newSize, address+28);
	 printf("New File Size: %d\n", newSize);
}

void TestWrite(){
//        const char* TempFileName = "XL_DATA    ";
//        cd("BLUE       ");
//        cd("BLUE2      ");
	const char* TempFileName = "A          ";
        int POSITION = 10;
        int NUM_BYTES = 4;
        char* string = "Nick";

	write(TempFileName,POSITION,NUM_BYTES, string);
	read(TempFileName,0,14);
}
