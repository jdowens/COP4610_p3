#include "write.h"

void write(const char* FILE_NAME, int POSITION, int NUM_BYTES, const char* STRING){
	FILE* ImageFile = GetImageFile();
	int cluster_number = NameToClusterNumber(FILE_NAME);
	struct DirectoryEntry* directoryArray = GetDirectoryContents(GetCurrentDirectoryClusterNum());


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
			printf("%s %d %d &s \n",directoryArray[index].DIR_Name,POSITION,NUM_BYTES,STRING);
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

	int left_in_cluster = directoryArray[index].DIR_FileSize;
	while(left_in_cluster >= 512){
        	left_in_cluster -= 512;
        }//end while
	left_in_cluster = 512-left_in_cluster;
	if(POSITION >= 512){
                //while statement will  go the the cluster with the starting position
                while(POSITION >= 512){
                        cluster_number = next_cluster(cluster_number);
                        POSITION = POSITION - 512;
                }//end while
        }//end if

	if(POSITION+NUM_BYTES >= 512){
		int bytes_write=512-POSITION;
		fseek(ImageFile,FindFirstSectorOfCluster(cluster_number)+POSITION,SEEK_SET);
                fwrite(STRING,1,bytes_write,ImageFile);//print number of bytes left_in_cluster
		cluster_number = next_cluster(cluster_number);
		NUM_BYTES -= bytes_write;
		while(cluster_number < 0x0ffffff8 && NUM_BYTES >= 512){
			fseek(ImageFile,FindFirstSectorOfCluster(cluster_number),SEEK_SET);
			fwrite(&STRING[bytes_write],sizeof(char),512,ImageFile);
                        NUM_BYTES -= 512;
			bytes_write += 512;
			cluster_number = next_cluster(cluster_number);
		}//end while

		if(POSITION+NUM_BYTES <= left_in_cluster){
                       	fseek(ImageFile,FindFirstSectorOfCluster(cluster_number)+POSITION,SEEK_SET);
                	fwrite(STRING,1,NUM_BYTES,ImageFile);//print number of bytes left_in_cluster
                }//end if
 
               else{
                        fseek(ImageFile,FindFirstSectorOfCluster(cluster_number),SEEK_SET);
                        fwrite(STRING,1,left_in_cluster,ImageFile);//print number of bytes left_in_cluster
                        NUM_BYTES -=left_in_cluster;
                        //write whole sectors at a time
                        cluster_number = next_cluster(cluster_number);
                        while(NUM_BYTES >= 512){
                                //call: find next free cluster
                                //call: update fat table function
                                //continue to write from the beginning of the new sector
                                fseek(ImageFile,FindFirstSectorOfCluster(cluster_number),SEEK_SET);
                                fwrite(&STRING[left_in_cluster],sizeof(char),512,ImageFile);
                                NUM_BYTES -= 512;
                                left_in_cluster +=512;
                                cluster_number = next_cluster(cluster_number);
                        }//end while
                                //call: find next free cluster
                                //call: update fat table function
                                //continue to write from the beginning of the new sector
                                fseek(ImageFile,FindFirstSectorOfCluster(cluster_number),SEEK_SET);
                                fwrite(&STRING[left_in_cluster],1,NUM_BYTES,ImageFile );
                }//end else
	}//end if
}//end write

void TestWrite(){
//        const char* TempFileName = "XL_DATA    ";
//        cd("BLUE       ");
//        cd("BLUE2      ");
	const char* TempFileName = "A          ";
        int POSITION = 0;
        int NUM_BYTES = 4;
        char* string = "Nick";

        write(TempFileName,POSITION,NUM_BYTES, string);
}

