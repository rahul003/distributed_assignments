#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <iostream>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <string>
#include <sstream>
#include <vector>

#include "process.h"
using namespace std;

int main(int argc, char *argv[])
{
    signal(SIGINT, my_handler);

    loadPorts();
    Process p2(2);
    boost::thread workerThread2(receive, &p2);
    sleep(5);
    // int i =1;
    p2.sendBroadcast(1);
    workerThread2.join();
    return 0; 
}
