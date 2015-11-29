#include "parseinput.h"

const char PREVDIR[11] = "..         ";
const char CURRDIR[11] = ".          ";

void ToFAT32(char * input)
{
   char FAT32string [13], temp [13];
   short inputSize = strlen(input);
   short count_spaces = 11 - inputSize;

   if(!strcmp(input, ".."))
   {
       strcpy(input, PREVDIR);
       return;
   }

   if(!strcmp(input, "."))
   {
       strcpy(input, CURRDIR);
       return;
   }

   for(int i = 0; i < inputSize; ++i)
   {
       if(input[i] >= 'a' && input[i] <= 'z')
           FAT32string[i] = input[i] - 32;
       else 
       {
           if (input[i] == '.' && inputSize < 12)
           {
               count_spaces = 11-(i+3);
           }

           FAT32string[i] = input[i];
       }
   }
   for(int i = inputSize; i < 12; ++i)
       FAT32string[i] = ' ';

   strcpy(temp, FAT32string);

   for(int i = 0; i < 11; ++i)
   {
       if(FAT32string[i] == '.')
       {
           FAT32string[8] = temp[i+1];
           FAT32string[9] = temp[i+2];
           FAT32string[10] = temp[i+3];
           for(int j = 0; j < count_spaces; ++j)
               FAT32string[i+j] = ' ';

           break;
       }

   } 
   FAT32string[11] = '\0';

   strcpy(input, FAT32string); 
}

void ToStandardFormat(char * input)
{
   if(!strcmp(input, PREVDIR))
   {
      strcpy(input, "..");
      return;
   }

   if(!strcmp(input, CURRDIR))
   {
      strcpy(input, ".");
      return;
   }


   char exten[4];
   char temp[12] = " ";

   exten[0] = input[8];
   exten[1] = input[9];
   exten[2] = input[10];
   exten[3] = '\0';

   for(int i = 0; i < 4; ++i)
       if(exten[i] == ' ')
           exten[i] = '\0';

   for(int i = 0; i < 11; i++)
   {
       if(input[i] == ' ')
       {
          if(exten[0] >= 'A' && exten[0] <= 'Z')
             temp[i] = '.';

          strcat(temp, exten);
          temp[i+strlen(exten)+1] = '\0';
          strcpy(input, temp);
          return;
       }

       temp[i] = input[i]; 
   }
   temp[11] = '\0';
   strcpy(input,temp);

}
