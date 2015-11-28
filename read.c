#include "read.h" 
void read(const char* FILE_NAME, int POSITION, int NUM_BYTES,unsigned int current_cluster){ 
/****************************************************************/ 
/*error if: file is unopened */ 
/* only for writing */ 
/* or if 
POSITION is greater than size of the file */ 
/****************************************************************/ 
//Check for end of cluster. 
//Is there an end of sector indicator or just read whole sector? unsigned int 
unsigned int cluster_number = current_cluster; while( ( cluster_number != 0x0ffffff8 ) ){
        unsigned char temp[NUM_BYTES];
        FILE* ImageFile = GetImageFile();
        //NOTE: Below uses the cluster number of the root directory(2)as a test.
        fseek(ImageFile,FindFirstSectorOfCluster(cluster_number),SEEK_SET);
        fread(temp,sizeof(unsigned char),NUM_BYTES,ImageFile);
        printf("TestRead:\n%s %d %d %x\n",FILE_NAME,POSITION,NUM_BYTES,FindFirstSectorOfCluster(cluster_number));
        printf("Array: %s\n",temp);
        cluster_number = next_cluster(cluster_number);
}
        printf("EoC = %x\n",cluster_number);
}
void TestRead(){
        const char* TempFileName = NULL;
        int current_cluster = 0x00000002;
        read(TempFileName,0,5,current_cluster);
}
