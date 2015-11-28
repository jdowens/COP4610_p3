#include "utility.h"
#include "filetable.h"
#include "directoryparse.h"

int main()
{
	OpenImageFile("fat32.img");
	ParseBootSector();
	TestFileTable();
	TestNextCluster();
	GetDirectoryContents(2);
	CloseImageFile();
	return 0;
}
