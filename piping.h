#ifndef PIPING_H
#define PIPING_H

#define DEFAULT_MAXPIPES 10
#define DEFAULT_MAXREDIRECTS 10
#define DEFAULT_MAXREADBUF 256
#define DEFAULT_MAXWRITEBUF 256

int piping(char **argv);

#endif