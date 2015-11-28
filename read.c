#include "read.h" 

void read(const char* FILE_NAME, int POSITION, int NUM_BYTES,unsigned int current_cluster){ 
/****************************************************************/ 
/*error if: file is unopened 					*/ 
/* only for writing 						*/ 
/* or if POSITION is greater than size of the file 		*/ 
/****************************************************************/ 

//Check for end of cluster. 
//Is there an end of sector indicator or just read whole sector? unsigned int 
unsigned int sector_size = 512;
unsigned int cluster_number = current_cluster; 
int byte_position = POSITION +  NUM_BYTES;
unsigned char temp[NUM_BYTES];
FILE* ImageFile = GetImageFile();
do{
	if(POSITION >= 512){
		while(POSITION >= 512){
			cluster_number = next_cluster(cluster_number);
			POSITION = POSITION - 512;
		}
		if(POSITION+NUM_BYTES >= 512){
			//go to position in sector
			fseek(ImageFile,FindFirstSectorOfCluster(cluster_number)+POSITION,SEEK_SET);
			//read entire sector
			fread(temp,sizeof(unsigned char),512,ImageFile);
			while(NUM_BYTES != 0){//if num_bytes =0 no more data to read
				next_cluster(cluster_number);
				NUM_BYTES = NUM_BYTES - 512;
				POSITION =0;
				//go to the beginning of the sector and read rest of data
				fseek(ImageFile,FindFirstSectorOfCluster(cluster_number)+POSITION,SEEK_SET);
                		//read entire sector
                		fread(temp,sizeof(unsigned char),512,ImageFile);
			}
		}
		else{
			//read from first cluster
               		fseek(ImageFile,FindFirstSectorOfCluster(cluster_number),SEEK_SET);
                	fread(temp,sizeof(unsigned char),NUM_BYTES,ImageFile);
			printf("Array: %s\n",temp);
		}
	}
	else{
		//read from first cluster
		fseek(ImageFile,FindFirstSectorOfCluster(cluster_number),SEEK_SET);
		fread(temp,sizeof(unsigned char),NUM_BYTES,ImageFile);
                //temp = read_data(POSITION,NUM_BYTES,current_cluster);
                printf("TestRead:\n%s %d %d %x\n",FILE_NAME,POSITION,NUM_BYTES,FindFirstSectorOfCluster(cluster_number));
                printf("Array: %s\n",temp);
		cluster_number = next_cluster(cluster_number);
	}
}while( ( cluster_number != 0x0ffffff8 ) );
        printf("EoC = %x\n",cluster_number);
}
/*
unsigned char read_data(int POSITION, int NUM_BYTES,unsigned int current_cluster){
	FILE* ImageFile = GetImageFile();
	unsigned char temp[NUM_BYTES];
	//go to the beginning of the sector and read rest of data
        fseek(ImageFile,FindFirstSectorOfCluster(current_cluster)+POSITION,SEEK_SET);
        //read entire sector
        fread(temp,sizeof(unsigned char),512,ImageFile);

	return (unsigned char temp);
}
*/
void TestRead(){
        const char* TempFileName = NULL;
        int current_cluster = 0x00000002;
        read(TempFileName,0,5,current_cluster);
}
