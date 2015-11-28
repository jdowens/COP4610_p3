#include "parseinput.h"
#include <stdio.h>
#include <stdlib.h>


void ToFAT32(char * input)
{
   char FAT32string [13], temp [13];
   short count_spaces = 11 - strlen(input);

   if(input[0] == '.')
   {
       printf ("illegal, DIR_Name[0] cannot be 0x20\n");
       return;
   }

   for(int i = 0; i < strlen(input); ++i)
   {
       if(input[i] >= 'a' && input[i] <= 'z')
           FAT32string[i] = input[i] - 32;
       else 
       {
           /*if (input[i] == '.' && strlen(input) < 12)
           {
               count_spaces++;
           }*/

           FAT32string[i] = input[i];
       }
   }
   for(int i = strlen(input); i < 12; ++i)
       FAT32string[i] = '*';

    printf("%s %i\n", FAT32string, count_spaces);
   strcpy(temp, FAT32string);

   for(int i = 0; i < strlen(FAT32string); ++i)
   {
       if(FAT32string[i] == '.')
       {
           FAT32string[8] = temp[i+1];
           FAT32string[9] = temp[i+2];
           FAT32string[10] = temp[i+3];
           for(int j = 0; j < count_spaces-1; ++j)
               FAT32string[i+j] = '*';

           break;
       }

   } 
   FAT32string[11] = '\0';

   printf("%s %i\n", FAT32string, strlen(FAT32string));
    

}

