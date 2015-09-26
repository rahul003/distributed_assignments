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
    Process p1(1);
    boost::thread workerThread1(receive, &p1);
    sleep(5);
    // int i =1;
    p1.sendBroadcast(1);
    workerThread1.join();
    return 0; 
}
