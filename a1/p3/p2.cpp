#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

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

#include "headers/process.h"
using namespace std;

int main(int argc, char *argv[])
{

    loadPorts();
     int id = 2;
    Process p2(id);
    string init = "Started process";
    p2.addToLog(init);
    //need all listening ports for p0: ports[i][id] for all i
    boost::thread_group workers;
    for (int i=0; i<ports.size();i++)
    {
        workers.add_thread(new boost::thread(receive, &p2, ports[i][id]));
    }
    sleep(3);
    p2.sendBroadcast(1);
    workers.join_all();
    return 0; 
}
