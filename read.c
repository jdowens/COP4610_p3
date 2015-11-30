#include "read.h" 

void read(const char* FILE_NAME, int POSITION, int NUM_BYTES){
	        int cluster_number = NameToClusterNumber(FILE_NAME);
		FILE* ImageFile = GetImageFile();
		unsigned char temp[512];
		struct DirectoryEntry* directoryArray = GetDirectoryContents(GetCurrentDirectoryClusterNum());
		unsigned int last_byte_loc =  NUM_BYTES-POSITION;
		int print_bytes= NUM_BYTES;

/*        	printf("\n\nTest Read Data Below: \n");
        	printf("Cluster Number for XL_DATA: %x\n",cluster_number);
        	printf("Cluster Address: %x\n",FindFirstSectorOfCluster(cluster_number));
*/
		//get dir names and sizes
	        int index = 0;
        	for(index = 0; !directoryArray[index].END_OF_ARRAY; index++){
                	//figure out how to read in the dir size to display
                	if( strcmp( FILE_NAME,directoryArray[index].DIR_Name ) == 0 ){
                        	printf("File Name: %s\n",directoryArray[index].DIR_Name);
                        	printf("File size: %d\n",directoryArray[index].DIR_FileSize);
                        	break;
                	}
        	}
        	if(directoryArray[index].END_OF_ARRAY){
               		printf("Did not find the name.\n");
			return;
        	}

		if( (POSITION > directoryArray[index].DIR_FileSize) || (NUM_BYTES > directoryArray[index].DIR_FileSize)){
			printf("Error: Out of File Size.\n");
		}
			
		else{
				if(POSITION >= 512){
					//while statement will  go the the cluster with the starting position
					while(POSITION >= 512){
	                        		cluster_number = next_cluster(cluster_number);
	                        		POSITION = POSITION - 512;
                			}//end while
				}//end if
				//read multi sectors
				if(POSITION+NUM_BYTES >= 512){
					int bytes_read = 512-POSITION;
					//go to position in sector
                       			fseek(ImageFile,FindFirstSectorOfCluster(cluster_number)+POSITION,SEEK_SET);
					//read to the end of sector then read from beginning of next sector
					fread(temp,sizeof(unsigned char),bytes_read,ImageFile);
                         		int index = 0;
                         		printf("Array in hex: ");
                         		for(index = 0; index < bytes_read; index++){
                                  		printf("%x ",temp[index]);
                         		}//end for
					cluster_number = next_cluster(cluster_number);
					NUM_BYTES = NUM_BYTES - bytes_read;
					while(cluster_number < 0x0ffffff8 && NUM_BYTES >= 512){
						//go to next position in sector
                                                fseek(ImageFile,FindFirstSectorOfCluster(cluster_number),SEEK_SET);
						//read entire sector
                                            	fread(temp,sizeof(unsigned char),512,ImageFile); 
                                        	for(index = 0; index < 512; index++){
                                                	printf("%x ",temp[index]);
                                        	}//end for
						cluster_number = next_cluster(cluster_number);
						NUM_BYTES = NUM_BYTES - 512;
					}//end while
                                        //go to next position in sector
                                       	fseek(ImageFile,FindFirstSectorOfCluster(cluster_number),SEEK_SET);
                                        //read entire sector
                                       	fread(temp,sizeof(unsigned char),NUM_BYTES,ImageFile);
	                                for(index = 0; index < NUM_BYTES; index++){
                                                printf("%x ",temp[index]);
                                        }//end for
					printf("\n");
				}//end if
				//read one sector only
				else{
					//read from only one sector
                			fseek(ImageFile,FindFirstSectorOfCluster(cluster_number),SEEK_SET);
                			fread(temp,sizeof(unsigned char),NUM_BYTES,ImageFile);
					for(index = 0; index < NUM_BYTES; index++){
                                                printf("%x ",temp[index]);
                                        }//end for
					printf("\n");
				}//end else
		}//end of else
}

void TestRead(){
	int NUM_BYTES = 7;
	int POSITION = 0;
	const char* TempFileName = "XL_DATA    ";
	cd("BLUE       ");
	cd("BLUE2      ");
	read(TempFileName,POSITION,NUM_BYTES);
}
