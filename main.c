#include "utility.h"
#include "filetable.h"

int main()
{
	OpenImageFile("fat32.img");
	ParseBootSector();
	TestFileTable();
	CloseImageFile();
	return 0;
}
