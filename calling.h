#ifndef CALLING_H
#define CALLING_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t call(char **argv);
int waitchild(pid_t pid,int *status);

#endif