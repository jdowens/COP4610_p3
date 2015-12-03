#include "rm.h"

const unsigned int EMPTY_BYTE = 0xE5;

void rm (char * fname)
{
    unsigned int firstClusterNum;
    unsigned int dirAddress;
    
    
    // *INSERT CHECK FOR FILE HERE*
    /*if()
    {
        printf("ERROR: %s is not a file in the current directory.\n", fname);
        return;
    }*/
    
    // get cluster number of the file
    firstClusterNum = NameToClusterNumber(fname);

    printf("firstClusterNum %i\n", firstClusterNum); 
    // find address of file in data and write empty to first byte
    dirAddress = FindFirstSectorOfCluster(firstClusterNum);
    printf("dirAddress %x\n", dirAddress); 
    WriteCharToImage(EMPTY_BYTE, dirAddress);
    printf("%i\n", firstClusterNum); 

    RemoveFromFAT(firstClusterNum);
    printf("firstFClusterNum %i\n", firstClusterNum); 
}

void rmdir (char * dirname)
{
    unsigned int firstClusterNum = NameToClusterNumber(dirname);
    unsigned int dirAddress = FindFirstSectorOfCluster(firstClusterNum);

    FILE* ImageFile = GetImageFile();
    char dirAtr[2] = " ";

    // check if it is actually a directory
    fseek(ImageFile, dirAddress, SEEK_SET);
    fread(dirAtr, sizeof(char), 1, ImageFile);

    if(dirAtr[0] == 0x10)
    {
        printf("ERROR: %s is not a directory.\n", dirname);
        return;
    }

    // *CHECK IF DIRECTORY HAS FILES IN IT*
    /*if()
    {
        printf("ERROR: Directory cannot be deleted as it contains a file(s).\n");
        return;
    }*/

    dirAddress = FindFirstSectorOfCluster(firstClusterNum);
    WriteCharToImage(EMPTY_BYTE, dirAddress);

    RemoveFromFAT(firstClusterNum);
}

void RemoveFromFAT(unsigned int cluster_number)
{
   // might need dynamic array here
   unsigned int clusterNums[512];
   unsigned short index = 0;
   unsigned int temp;

   do
   {
       // add cluster numbers to stack
       temp = next_cluster(cluster_number);
       clusterNums[index] = temp;
       index++;
   }
   while(temp != 0x0FFFFFF8);

   // pop cluster numbers and set to 0
   for(int i = index-1; i >= 0; --i)
   {
       WriteToFAT(clusterNums[i], 0);
   }
}

