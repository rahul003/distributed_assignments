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



void error(char *msg)
{
    perror(msg);
    exit(1);
}

string iToString(int s)
{
    ostringstream lconvert;
    lconvert.str("");
    lconvert<<s;
    return lconvert.str();
}

string doubleToString(double s)
{
    ostringstream lconvert;
    lconvert.str("");
    lconvert<<s;
    return lconvert.str();
}

//fns to to split string
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}




vector<int> getVectorClock_vec(string s)
{
    vector<int> rv;
    vector<string> tokens = split(s, '.');
    for(std::vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it) 
    {
        rv.push_back(atoi( (*it).c_str() ));
    }
    return rv;
}

int getSender(string s)
{
    vector<int> rv;
    vector<string> tokens = split(s, ',');
    return atoi(tokens[0].c_str());
}

int getMessageNum(string s)
{
    vector<int> rv;
    vector<string> tokens = split(s, ',');
    return atoi(tokens[1].c_str());   
}

string getMessageVC_str(string s)
{
    vector<int> rv;
    vector<string> tokens = split(s, ',');
    return tokens[2];      
}

vector< vector<int> > ports;

void loadPorts()
{
    string filepath = "/home/rahul/git/distributed/a1/p3/text/ports.txt";
    int numrows = 0;
    int numcols = 0;
    string portlinet;
    ifstream portdatax(filepath.c_str());
    if(portdatax.is_open())
    {
        while(getline(portdatax, portlinet))
        {
            if (numcols==0)
                {numcols = split(portlinet, ',').size();}
            numrows++;
        }
    }
    else
    {
        cout<<"failed to open file"<<endl;
    }
    portdatax.close();
    vector<int> x;
    for (int i=0; i<numrows; i++)
    {   
        ports.push_back(x);
    }


    string portline;
    int portnum;
    ifstream portdata(filepath.c_str());
    if (portdata.is_open())
      {
        int linenum = 0;
        while (getline(portdata,portline))
        {
          vector<string> tokens = split(portline, ',');
          
          for(std::vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it) 
            {
                portnum = atoi((*it).c_str());
                ports[linenum].push_back(portnum);
            }
          linenum++;
        }
        portdata.close();
      }

    else
    {
        cout<<"failed to open file"<<endl;
    }
      // cout<<"Can it print here"<<endl;
      // cout<<ports[0][0]<<endl;
    return;
}


vector<vector<int> > loadDelay()
{
    vector<vector <int> > delay;

    string filepath = "/home/rahul/git/distributed/a1/p3/text/delay.txt";
    int numrows = 0;
    int numcols = 0;
    string t;
    ifstream delay_data(filepath.c_str());
    if(delay_data.is_open())
    {
        while(getline(delay_data, t))
        {
            if (numcols==0)
                {numcols = split(t, ' ').size();}
            numrows++;
        }
    }
    else
    {
        cout<<"failed to open file"<<endl;
    }
    delay_data.close();

    vector<int> x;
    for (int i=0; i<numrows; i++)
    {   
        delay.push_back(x);
    }

    string line;
    int numdelay;
    ifstream delay_data(filepath.c_str());
    if (delay_data.is_open())
      {
        int linenum = 0;
        while (getline(delay_data,line))
        {
          vector<string> tokens = split(line, ' ');
          
          for(std::vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it) 
            {
                numdelay = atoi((*it).c_str());
                delay[linenum].push_back(numdelay);
            }
          linenum++;
        }
        delay_data.close();
      }

    else
    {
        cout<<"failed to open file"<<endl;
    }
    return delay;
}