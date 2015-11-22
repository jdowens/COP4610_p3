#include "utility.h"
#include "filetable.h"

int main()
{
	OpenImageFile("fat32.img");
	ParseBootSector();
	FindRootDirectory();
	TestFileTable();
	CloseImageFile();
	return 0;
}
