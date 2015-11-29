#include "utility.h"
#include "filetable.h"
#include "directoryparse.h"
#include "parseinput.h"
#include "ls.h"
#include "program.h"
#include "read.h"

int main()
{
//	OpenImageFile("fat32.img");
//	ParseBootSector();
//	TestFileTable();
//	TestNextCluster();

	//list(2);
	//list(4);
    //list(5);
	OpenImageFile("fat32.img");
	ParseBootSector();
	TestFileTable();
	TestNextCluster();
	SetCurrentDirectoryClusterNum(GetRootClusterNum());
	list(2);
	list(NameToClusterNumber("BLUE       "));
	size("FATSPEC PDF");
	list(4);
	list(5);
	char test[20] = "pickles.asf";
    ToFAT32(test);
    	printf("KAJSDKFJALSKDJF: %s %i\n", test, strlen(test));
	/*
	// print root
	// and how to use GetDirectoryContents fucnction
	printf("\n\n\n\n\nPRINT ROOT DIRECTORY:\n");
	struct DirectoryEntry* tmp = GetDirectoryContents(2);
	int index = 0;
	while (!tmp[index].END_OF_ARRAY)
	{
		printf("TEST_NAME: %s\n",tmp[index++].DIR_Name);
	}
	// print RED
	// printf("\n\n\n\n\nPRINT RED DIRECTORY:\n");
	//GetDirectoryContents(3);
	// print GREEN
	printf("\n\n\n\n\nPRINT GREEN DIRECTORY:\n");
	GetDirectoryContents(4);
	// print BLUE
	printf("\n\n\n\n\nPRINT BLUE DIRECTORY:\n");
	GetDirectoryContents(5);
	*/
	//CloseImageFile();
//	RunProgram();
	TestRead();
	return 0;
}
