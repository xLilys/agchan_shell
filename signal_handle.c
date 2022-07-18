#include "signal_handle.h"


signalhandler_t trap_signal(int sig,signalhandler_t handler){
    struct sigaction act,old;
    
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_RESTART;
    if(sigaction(sig,&act,&old) < 0)return;
    return old.sa_handler;
}



