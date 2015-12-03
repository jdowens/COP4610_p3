#ifndef _RM_H
#define _RM_H

#include "utility.h"
#include "directoryparse.h"

void rm (char * fname);
void rmdir (char * dirname);
void RemoveFromFAT (unsigned int cluster_number);

#endif
