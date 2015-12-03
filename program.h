#ifndef _PROGRAM_H
#define _PROGRAM_H

#include <stdio.h>
#include <string.h>
#include "directoryparse.h"
#include "utility.h"
#include "cd.h"
#include "openclose.h"
#include "ls.h"
#include "size.h"
#include "read.h"
#include "parseinput.h"
#include "create.h"
#include "mkdir.h"
#include "rm.h"


#define USER_INPUT_BUFFER_LENGTH 256

void RunProgram(void);
void PrintPrompt(void);
void GetUserInput(void);

#endif
