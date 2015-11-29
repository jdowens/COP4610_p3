#include "read.h" 

void read(const char* FILE_NAME, int POSITION, int NUM_BYTES,unsigned int current_cluster){ 
		FILE* ImageFile = GetImageFile();
		unsigned int cluster_number = current_cluster;
		unsigned char temp[NUM_BYTES];
		struct DirectoryEntry* directoryArray = GetDirectoryContents(GetCurrentDirectoryClusterNum());
		unsigned int last_byte_loc =  NUM_BYTES-POSITION;

		if( (POSITION > directoryArray[0].DIR_FileSize) || (NUM_BYTES > directoryArray[0].DIR_FileSize)){
			printf("Error: Out of File Size.\n");
		}
		else{
			do{
				if(POSITION >= 512){
					//while statement will  go the the cluster with the starting position
					while(POSITION >= 512){
	                        		cluster_number = next_cluster(cluster_number);
	                        		POSITION = POSITION - 512;
                			}//end while
				}//end if

				if(POSITION+NUM_BYTES >= 512){
					//This while loop will read EnTIre SEctORs!
					while(NUM_BYTES > 512){
						//go to position in sector
                        			fseek(ImageFile,FindFirstSectorOfCluster(cluster_number)+POSITION,SEEK_SET);
						//check for cases like POSITION = 511 and read 100 more bytes
						//need to read until end of sector then get next cluster.
						if( (POSITION+NUM_BYTES) > 512){
							fread(temp,sizeof(unsigned char),512-POSITION,ImageFile);
							cluster_number = next_cluster(cluster_number);
							while(cluster_number < 0x0ffffff8){
								POSITION = 0;
								//go to next position in sector
	                                                	fseek(ImageFile,FindFirstSectorOfCluster(cluster_number)+POSITION,SEEK_SET);
								//read entire sector
                                                		fread(temp,sizeof(unsigned char),512,ImageFile);
								cluster_number = next_cluster(cluster_number);
							}
							POSITION = 0;
                                                        //go to next position in sector
                                                         fseek(ImageFile,FindFirstSectorOfCluster(cluster_number)+POSITION,SEEK_SET);
                                                         //read entire sector
                                                         fread(temp,sizeof(unsigned char),512,ImageFile);
						}//end if

					}//end while
					//go to position in sector
				}//end if

				else{//only in the first sector (POSITION+NUM_BYTES < 512)
					//read from first cluster
                			fseek(ImageFile,FindFirstSectorOfCluster(cluster_number),SEEK_SET);
                			fread(temp,sizeof(unsigned char),NUM_BYTES,ImageFile);
                			printf("TestRead:\n%s %d %d %x\n",FILE_NAME,POSITION,NUM_BYTES,FindFirstSectorOfCluster(cluster_number));
					printf("last_byte_loc: %d\n",last_byte_loc);
					printf("Array: %s\n",temp);
 	              			cluster_number = next_cluster(cluster_number);
				}
			}while( ( cluster_number < 0x0ffffff8 ) );
		}//end of else
}

void TestRead(){
        const char* TempFileName = NULL;
	struct DirectoryEntry* directoryArray = GetDirectoryContents(GetCurrentDirectoryClusterNum());
	cd("BLUE       ");
	cd("BLUE2      ");
        int current_cluster = NameToClusterNumber("XL_DATA    ");
        printf("\nTest Read Data Below: \n");
        printf("Cluster Number for XL_DATA: %x\n",current_cluster);
        printf("Cluster Address: %x\n",FindFirstSectorOfCluster(current_cluster));
       //figure out how to read in the dir size to display
	printf("File A size: %d\n",directoryArray[0].DIR_FileSize);
	read(TempFileName,0,7,current_cluster);
}

