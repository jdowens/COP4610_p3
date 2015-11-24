#include "ls.h"

void list(){

	FILE* ImageFile = NULL;
	
	
	ImageFile = fopen(name, "r+b");
	if (ImageFile == NULL)
        {
                printf("Image file could not be opened...\n");                  
                exit(-1);
        } 




	//set up array to store names of directories and files to print
	//char * ls_print = malloc(10 * sizeof(char));

	//check if this is a valid directory name
	//check bytes 26-27 for the starting cluster
	//locate the cluster number
	//given where root starts

	//parse every 32 bytes
	//print that out

	//parse next 32 bytes

	//16 entries = 1 sector
	//read more
	//find next cluster
	//iterate over every file in cluster. If not end of cluster
	//go to next cluster
	//512 bytes per sector and cluster
	//given cluster number, where is next number
	//convent between array of chars and big endian
	 fseek(ImageFile, 44, SEEK_SET);
         fread(store_bytes, sizeof(char), 4, ImageFile);
         BPB_RootClus = store_bytes[0];
	 printf("RootClus = %i\n", BPB_RootClus);

	








}




