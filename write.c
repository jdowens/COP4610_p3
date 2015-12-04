#include "write.h"

const char* SPACE=" ";

void write(const char* FILE_NAME, int POSITION, int NUM_BYTES, const char* STRING){
	int cluster_number = NameToClusterNumber(FILE_NAME);
	struct DirectoryEntry* directoryArray = GetDirectoryContents(GetCurrentDirectoryClusterNum());
	int index=0;
	int num_spaces=0;

	//if no errors get the index of the file
	index = error_check(FILE_NAME,directoryArray,POSITION,NUM_BYTES,STRING);
	unsigned int old_FileSize = directoryArray[index].DIR_FileSize;
	unsigned int address = directoryArray[index].DIR_EntryByteAddress;
	printf("A byte address: %x\n", FindFirstSectorOfCluster(cluster_number));

	if(POSITION > directoryArray[index].DIR_FileSize){
		num_spaces = POSITION - directoryArray[index].DIR_FileSize;
		int spacing_index;
                for(spacing_index=0; spacing_index < num_spaces; spacing_index++){
                        write_at_position(0,old_FileSize+spacing_index,1,cluster_number,SPACE);
                }//end for
	}//end if

        //add the number of written bytes to file size
        update_filesize(address,directoryArray,index,NUM_BYTES,POSITION, num_spaces);

	if(POSITION >= 512){
                //while statement will  go the the cluster with the starting position
                while(POSITION >= 512){
                        cluster_number = next_cluster(cluster_number);
                        POSITION = POSITION - 512;
                }//end while
        }//end if

	if(POSITION+NUM_BYTES >= 512){
		int bytes_write=512-POSITION;
		write_at_position(0,POSITION,bytes_write,cluster_number,STRING);
		NUM_BYTES -= bytes_write;
		cluster_number = next_cluster(cluster_number);
		while(cluster_number < 0x0ffffff8 && NUM_BYTES >= 512){
			write_at_position(bytes_write,0,512,cluster_number,STRING);
			bytes_write += 512;
			NUM_BYTES -= 512;
			cluster_number = next_cluster(cluster_number);
		}//end while
		if(NUM_BYTES <= 512){//filled out the last cluster chain and no more bytes
			 write_at_position(bytes_write,0,512,cluster_number,STRING);//update file size
			return;
		}//end if
		else{//write to end of the last cluster chain
			write_at_position(bytes_write,0,512,cluster_number,STRING);
                        bytes_write += 512;
                        NUM_BYTES -= 512;
			//add nother cluster and continue to write
			while(NUM_BYTES >= 512){
                                add_cluster(cluster_number);
                                write_at_position(bytes_write,0,512,cluster_number,STRING);
                                NUM_BYTES -= 512;
                                bytes_write +=512;
                                cluster_number = next_cluster(cluster_number);
                        }//end while
                                add_cluster(cluster_number);
                                write_at_position(bytes_write,0,NUM_BYTES,cluster_number,STRING);
		}//end else
	}//end if
	else{
		write_at_position(0,POSITION,NUM_BYTES,cluster_number,STRING);
		//update file size
	}//end else
}//end write

void add_cluster(int cluster_number){
	int next_free_cluster = FindNextFreeCluster();//find next free cluster
	next_free_cluster = 0x0ffffff8;//set it to end of chain
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
        NUM_BYTES += num_spaces;//add the number of spaces to the total
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
        }

	 WriteIntToImage(directoryArray[index].DIR_FileSize,address+28);
	 printf("New File Size: %s %d\n",directoryArray[index].DIR_Name,directoryArray[index].DIR_FileSize);
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
