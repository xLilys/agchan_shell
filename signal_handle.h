#ifndef SIGHANDLE_H
#define SIGHANDLE_H

#include <signal.h>

typedef void (*signalhandler_t)(int);
signalhandler_t trap_signal(int sig,signalhandler_t handler);


#endif