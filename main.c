#include "utility.h"
#include "filetable.h"
#include "directoryparse.h"

int main()
{
	OpenImageFile("fat32.img");
	ParseBootSector();
	TestFileTable();
	TestNextCluster();
	// print root
	printf("\n\n\n\n\nPRINT ROOT DIRECTORY:\n");
	GetDirectoryContents(2);
	// print RED
	// printf("\n\n\n\n\nPRINT RED DIRECTORY:\n");
	//GetDirectoryContents(3);
	// print GREEN
	printf("\n\n\n\n\nPRINT GREEN DIRECTORY:\n");
	GetDirectoryContents(4);
	// print BLUE
	printf("\n\n\n\n\nPRINT BLUE DIRECTORY:\n");
	GetDirectoryContents(5);
	CloseImageFile();
	return 0;
}
