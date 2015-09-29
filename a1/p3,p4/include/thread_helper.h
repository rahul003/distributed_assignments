#ifndef THREAD_HELPER    // To make sure you don't declare the function more than once by including the header multiple times.
#define THREAD_HELPER

#include "process.h"
void write_log(Process* parent);
void psuedoreceive_manager(Process* parent);

#endif