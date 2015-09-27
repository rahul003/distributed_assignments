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

void write_log(Process* parent)
{
    string filepath = "/home/rahul/git/distributed/a1/p3/logs/log_";
    filepath += iToString(parent->getId());
    
while(1)
{
    ofstream outfile(filepath.c_str());
    if (outfile.is_open())
    {
        map <double, vector<string> > p_log = parent->getLog();
        for (const auto& p : p_log)
        {
            outfile<<"Time: "<<p.first<<endl;
            for(std::vector<string>::iterator it = p.second.begin(); it != p.second.end(); ++it) 
            {
                
                outfile<<(*it)<<endl;
            }
            outfile<<endl;
        }
    }
    outfile.close();
    sleep(0.5);
 }
    //for every 0.5s, write to file
    return;
}

void psuedoreceive_manager(Process* parent)
{
    while(1)
    {
        map <double, vector<string> > * psr = parent->getPsuedoreceived();
        for (const auto& p : *psr)
        {
            double curtimeoff = difftime(time(0),parent->getStartTime());
            if (p.first<=curtimeoff)
            {
                for(std::vector<string>::iterator it = p.second.begin(); it != p.second.end(); ++it) 
                {
                    parent->addToNotdelivered(p.first,(*it));

                vector<string> tokens = split((*it), ',');    
                string toLog();
                toLog += "P";
                toLog += iToString(parent->getId());
                toLog += " REC p_";
                toLog += tokens[0];
                toLog += ":";
                toLog += tokens[1];
                double cutimeoff = difftime(time(0),parent->getStartTime());
                parent->addToLog(curtimeoff, toLog);

                parent->deliveryManager();
                    //add to log that received
                }
                *psr.erase(curtimeoff);
            }
        }
        sleep(1);
    }
    return;
}

int main(int argc, char *argv[])
{
    loadPorts();
    int id = 0;
    Process p0(id);
    string init = "Started process";
    p0.addToLog(init);
    boost::thread_group workers;
    for (int i=0; i<ports.size();i++)
    {
        workers.add_thread(new boost::thread(receive, &p0, ports[i][id]));
    }
    boost::thread logger_thread(write_log, &p0);
    boost::thread psuedoreceive_mgr_thread(psuedoreceive_manager, &p0);

    sleep(3);
    string output = p0.getVectorClock_str();
        
    p0.sendBroadcast(1);
    workers.join_all();
    return 0; 
}
