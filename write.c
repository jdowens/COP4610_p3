#include "write.h"

void write(const char* FILE_NAME, int POSITION, int NUM_BYTES, const char * STRING){
	if(FTIsOpenInRead(FILE_NAME)){
		printf("Error: Cannot access file.");
	}//end if

	//--->check it is not a directory<---

	int left_in_cluster = size(FILE_NAME);
	//finds out how much space is left in the cluster
	//assuming all clusters until the last one is filled
	if(size >= 512){
		left_in_cluster -= 512;
	}//end if

	if(POSITION >= 512){
		//while statement will  go the the cluster with the starting position
		while(POSITION >= 512){
            		cluster_number = next_cluster(cluster_number);
	                POSITION = POSITION - 512;
                }//end while
	}//end if
	
	//got to the position
	fseek(ImageFile,FindFirstSectorOfCluster(cluster_number)+POSITION,SEEK_SET);
	if(POSITION+NUM_BYTES>= 512){
		int i=0;
		int read_bytes = 512 - POSITION;//512-511 = 1 more byte to read
		NUM_BYTES -= POSITION;//bytes left to write
		for(i=0; i < read_bytes; i++){
			fprintf(FILE_NAME,"%s",STRING);
		}//end for
		//print to while sectors
		while(NUM_BYTES >= 512){
			cluster_number = next_cluster(cluster_number);
			fseek(ImageFile,FindFirstSectorOfCluster(cluster_number),SEEK_SET);
			fprintf(FILE_NAME,"%s",STRING);
			NUM_BYTES -= NUM_BYTES;
		}
		//print rest in the next sector
		cluster_number = next_cluster(cluster_number);
		fseek(ImageFile,FindFirstSectorOfCluster(cluster_number),SEEK_SET);
		fprintf(FILE_NAME,"%s",STRING);
	}
	//print to only 1 sector
	else{
		fprintf(FILE_NAME,"%s",STRING);
	}
}

void TestWrite(){
	const char* TempFileName = "XL_DATA    ";
	int POSITION = 0;
	int NUM_BYTES = 10;
	const char* string[NUM_BYTES] = "Nick";
	cd("BLUE       ");
	cd("BLUE2      ");
	write(TempFileName,POSITION,NUM_BYTES, string);
}
