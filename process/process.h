#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

void forkIt(char **args, int background);

#endif
