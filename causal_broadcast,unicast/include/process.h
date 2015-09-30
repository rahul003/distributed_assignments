/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
//g++ -w server.cpp -o server -lboost_thread-mt
#ifndef PROCESS    // To make sure you don't declare the function more than once by including the header multiple times.
#define PROCESS

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
#include <fstream>
#include <vector>
#include <ctime>
#include <time.h>

#include "helper.h"

using namespace std;

typedef map<double, vector<string> >::iterator it_map;

class Process{
    public:
        //handle both
        Process(int, string);
        string getVectorClock_str();
        void updateVectorClock_receive(string);
        
        //common
        int getId();
        int getPortOf(int);
        vector< vector<int> > getDelay();
        time_t getStartTime();
        map <double, vector<string> > * getPsuedoreceived();
        map <double, vector<string> >  getLog();
        void addToPsuedoreceived(double, string);
        void addToNotdelivered(double , string);
        void addToLog(double , string);
        string getCommtype();
        //receive as well
        
        //separate
        void deliveryManager();
        void sendBroadcast(int);
        
        void updateVectorClock_sendBc();
        
        
        //done. separate
        void sendUnicast(int, int);
        void updateVectorClock_sendUc(int);
        string getRec_str();

    private:
        int myid;
        time_t t_start;
        map <double, vector<string> > psuedoreceived;
        map <double, vector<string> > my_log;
        map <double, vector<string>, greater<double> > notdelivered;
        vector< vector<int> > delay;
        string commtype;

        vector <int> vclock;
        vector <int> D;

        vector< vector<int> > sent;
        vector<int> rec;
};

void receive (Process *, int);


#endif
