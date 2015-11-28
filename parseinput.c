#include "parseinput.h"
#include <stdio.h>
#include <stdlib.h>


void ToFAT32(char * input)
{
   char FAT32string [13];

   if(input[0] == '.')
   {
       printf ("illegal, DIR_Name[0] cannot be 0x20\n");
       return;
   }

   for(int i = 0; i < strlen(input); ++i)
       FAT32string[i] = input[i];

   for(int i = strlen(input); i < 12; ++i)
       FAT32string[i] = ' ';

   for(int i = 0; i < strlen(FAT32string); ++i)
   {
       if(FAT32string[i] > 'a' && FAT32string[i] < 'z')
           FAT32string[i] = FAT32string[i] - 32;

       else if(FAT32string[i] == '.')
       {
           FAT32string[9] = FAT32string[i+1];
           FAT32string[10] = FAT32string[i+2];
           FAT32string[11] = FAT32string[i+3];
       }

   } 
   FAT32string[12] = '\0';

   printf("%s\n", FAT32string);
    

}

