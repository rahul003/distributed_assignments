#ifndef HELPER    // To make sure you don't declare the function more than once by including the header multiple times.
#define HELPER


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

using namespace std;
//common
void error(char *);
string iToString(int );
string doubleToString(double );
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim) ;
int getSender(string );
int getMessageNum(string );
void loadPorts();
vector<vector<int> > loadDelay(string);
string getMessageVC_str(string );

//bc
vector<int> getVectorClock_vec(string );
map<int, vector<int> > loadBc();

//uc
vector< vector<int> > getVectorClock_matrix(string );
map<int, vector<int> > loadUc(int);

extern vector< vector<int> > ports;



#endif