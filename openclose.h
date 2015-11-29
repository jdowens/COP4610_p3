#ifndef _OPENCLOSE_H
#define _OPENCLOSE_H

#include "filetable.h"
#include "directoryparse.h"

void open(const char* file_name, const char* mode);
void close(const char* file_name);

#endif
