#include "rootparse.h"
#include <stdio.h>
#include <stdlib.h>



//rootparse.c


//go through 32 bytes
//parse this data

void rootparsing()
{
	//root start
	FILE* datafile = GetImageFile();
	unsigned int root_cluster = 2;
	unsigned int cluster_number = 0;
	unsigned int byte_number = cluster_number * GetBytesPerSector() 
* GetSectorsPerCluster();

	char data_from_root[32];

	fseek(datafile, byte_number, SEEK_SET);
	fread(data_from_root, sizeof(char), 32, datafile);

	 

	




}

