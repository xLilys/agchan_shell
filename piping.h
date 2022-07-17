#ifndef PIPING_H
#define PIPING_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_MAXPIPES 10
#define DEFAULT_MAXREDIRECTS 10
#define DEFAULT_MAXREADBUF 256
#define DEFAULT_MAXWRITEBUF 256

int piping(char **argv);

#endif