#include "utility.h"

int main()
{
	OpenImageFile("fat32.img");
	ParseBootSector();
	FindRootDirectory();
}
