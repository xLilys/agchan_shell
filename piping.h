#ifndef PIPING_H
#define PIPING_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_MAXPIPES 10
#define OUTPUT_BUFSIZE 128

int piping(char **argv,char **output,char t);

#endif