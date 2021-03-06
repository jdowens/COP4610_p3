#include "read.h"

void read(const char* FILE_NAME, int POSITION, int NUM_BYTES){
		int bytes_per_sec = GetBytesPerSec()*GetSecPerClus();
		FILE* ImageFile = GetImageFile();
	        int cluster_number = NameToClusterNumber(FILE_NAME);
		unsigned char temp[512];
		struct DirectoryEntry* directoryArray = GetDirectoryContents(GetCurrentDirectoryClusterNum());

		if(FTIsOpen(FILE_NAME)==0){
			printf("Error: File is not open.\n");
                        return;
		}
                if(FTIsOpenInRead(FILE_NAME)==0){
                        printf("Error: Cannot access this file.\n");
                        return;
               }

                //get dir names and sizes
                int index = 0;
                for(index = 0; !directoryArray[index].END_OF_ARRAY; index++){
                        if( strcmp( FILE_NAME,directoryArray[index].DIR_Name ) == 0 ){
                                //printf("File Name: %s\n",directoryArray[index].DIR_Name);
                                //printf("File size: %d\n\n",directoryArray[index].DIR_FileSize);
                                break;
                        }
                }

        	if(directoryArray[index].END_OF_ARRAY){
               		printf("Error: Did not find the name.\n");
			return;
        	}

		if( (POSITION > directoryArray[index].DIR_FileSize) || (NUM_BYTES > directoryArray[index].DIR_FileSize) || ( (POSITION+NUM_BYTES) > directoryArray[index].DIR_FileSize )){
			printf("Error: Out of File Size.\n");
			return;
		}

		else{
				if(POSITION >= bytes_per_sec){
					//while statement will  go the the cluster with the starting position
					while(POSITION >= bytes_per_sec){
	                        		cluster_number = next_cluster(cluster_number);
	                        		POSITION = POSITION - bytes_per_sec;
                			}//end while
				}//end if
				//read multi sectors
				if(POSITION+NUM_BYTES >= bytes_per_sec){
					int bytes_read = bytes_per_sec-POSITION;
					//go to position in sector
                       			fseek(ImageFile,FindFirstSectorOfCluster(cluster_number)+POSITION,SEEK_SET);
					//read to the end of sector then read from beginning of next sector
					fread(temp,sizeof(unsigned char),bytes_read,ImageFile);
                         		int index = 0;
                         		for(index = 0; index < bytes_read; index++){
                                  		printf("%c",temp[index]);
                         		}//end for
					cluster_number = next_cluster(cluster_number);
					NUM_BYTES = NUM_BYTES - bytes_read;
					while(cluster_number < 0x0ffffff8 && NUM_BYTES >= bytes_per_sec){
						//go to next position in sector
                                                fseek(ImageFile,FindFirstSectorOfCluster(cluster_number),SEEK_SET);
						//read entire sector
                                            	fread(temp,sizeof(unsigned char),bytes_per_sec,ImageFile);
                                        	for(index = 0; index < bytes_per_sec; index++){
                                                	printf("%c",temp[index]);
                                        	}//end for
						cluster_number = next_cluster(cluster_number);
						NUM_BYTES = NUM_BYTES - bytes_per_sec;
					}//end while
                                        //go to next position in sector
                                       	fseek(ImageFile,FindFirstSectorOfCluster(cluster_number),SEEK_SET);
                                        //read entire sector
                                       	fread(temp,sizeof(unsigned char),NUM_BYTES,ImageFile);
	                                for(index = 0; index < NUM_BYTES; index++){
                                                printf("%c",temp[index]);
                                        }//end for
				}//end if
				//read one sector only
				else{
					//read from only one sector
                			fseek(ImageFile,FindFirstSectorOfCluster(cluster_number)+POSITION,SEEK_SET);
                			fread(temp,sizeof(unsigned char),NUM_BYTES,ImageFile);
					for(index = 0; index < NUM_BYTES; index++){
                                                printf("%c",temp[index]);
                                        }//end for
				}//end else
		}//end of else
}


/* Compare file name then CD and recompare until file found. */
void valid_file(const char* FILE_NAME){	
	struct DirectoryEntry* directoryArray = GetDirectoryContents(GetCurrentDirectoryClusterNum());
        int root_index = 0;
	int cd_index = 0;
	int dir_index = 0;

	for(root_index = 0; !directoryArray[root_index].END_OF_ARRAY; root_index++){
		printf("%s %x",directoryArray[root_index].DIR_Name,directoryArray[dir_index].DIR_Attr);
		if( strcmp( FILE_NAME,directoryArray[root_index].DIR_Name ) == 0 ){
                	printf("File Found =D");
                	return;
                }//end if
		else{
			//I think this checks if name is a directory
			while(directoryArray[dir_index].DIR_Attr & 0x10 && !directoryArray[root_index].END_OF_ARRAY){
				printf("%s ",directoryArray[root_index].DIR_Name);
				cd(directoryArray[dir_index].DIR_Name);
				for(cd_index = 0; !directoryArray[cd_index].END_OF_ARRAY; cd_index++){
					if( strcmp( FILE_NAME,directoryArray[cd_index].DIR_Name ) == 0 ){
		                        	printf("File Found =D");
                		               	return;
					}//end if
				}//end for
				dir_index++;
			}//end while
		}//end else
	}//end for
	printf("\nError: File not found.\n");
}



void TestRead(){
	int NUM_BYTES = 10;
	int POSITION = 0;
//	const char* TempFileName = "XL_DATA    ";
//	cd("BLUE       ");
//	cd("BLUE2      ");
//	valid_file(TempFileName);
	const char* TempFileName = "A          ";
	read(TempFileName,POSITION,NUM_BYTES);
}
