#include "read.h"

read(FILE_NAME, unsigned short POSITION){
//error if:     file is unopened
//              only for writing
//              or if POSITION is greater than size of the file

//1. Find file location in FAT
//2. Start at POSITION
//3. Count bytes

int NUM_BYTES = 0;
unsigned short end[1];

        fseek(ImageFile,POSITION,SEEK_SET);
        while(end =! ){
		fread(end,sizeof(char),1,ImageFile);
		printf("%i",end);
                NUM_BYTES++;
        }
}


