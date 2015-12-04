#ifndef _WRITE_H
#define _WRITE_H
#include <string.h>
#include <stdio.h>
#include "cd.h"
#include "filetable.h"
#include "size.h"
#include "utility.h"
#include "parseinput.h"
#include "read.h"

void write(const char* FILE_NAME, int POSITION, int NUM_BYTES, const char* STRING);
void add_cluster(int cluster_number);
void write_at_position(int string_position,int POSITION,int written_bytes,int cluster_number,const char* STRING);
int error_check(const char* FILE_NAME,struct DirectoryEntry* directoryArray,int POSITION,int NUM_BYTES,const char* STRING);
void TestWrite();

#endif
