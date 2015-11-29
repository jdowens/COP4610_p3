#include "read.h" 

void read(const char* FILE_NAME, int POSITION, int NUM_BYTES,unsigned int current_cluster){ 
		FILE* ImageFile = GetImageFile();
		unsigned int cluster_number = current_cluster;
		unsigned char temp[NUM_BYTES];

		//read from first cluster
                fseek(ImageFile,FindFirstSectorOfCluster(cluster_number),SEEK_SET);
                fread(temp,sizeof(unsigned char),NUM_BYTES,ImageFile);
                //temp = read_data(POSITION,NUM_BYTES,current_cluster);
                printf("TestRead:\n%s %d %d %x\n",FILE_NAME,POSITION,NUM_BYTES,FindFirstSectorOfCluster(cluster_number));
                printf("Array: %s\n",temp);
                //cluster_number = next_cluster(cluster_number);
}

void TestRead(){
        const char* TempFileName = NULL;
//        int current_cluster = 0x00000002;
//      read(TempFileName,0,5,current_cluster);
        int current_cluster = NameToClusterNumber("A          ");
        printf("Test Read Data Below: \n");
        printf("Cluster Number for A: %x\n",NameToClusterNumber("A          "));
        printf("Cluster Address: %x\n",FindFirstSectorOfCluster(current_cluster));
       //figure out how to read in the dir size to display
	 printf("File A size: %d",directoryArray[0].DIR_Size);
	read(TempFileName,0,8,current_cluster);
}
