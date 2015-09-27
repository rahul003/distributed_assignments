/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
//g++ -w server.cpp -o server -lboost_thread-mt
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
#include "headers/helper.h"

using namespace std;

class Process{
    public:
        Process(int);
        int getId();
        int getData();
        void setData(int);
        int getPortOf(int);
        // void sendMsg(int, int);
        void sendBroadcast(int);
        // void addToLog(string);
        string getVectorClock_str();
        void updateVectorClock_send();
        void updateVectorClock_receive(string);
        time_t getStartTime();
        void addToPsuedoreceived(double, string);
        void addToNotdelivered(double , string);
        void addToLog(double , string);
        map <double, vector<string> > * getPsuedoreceived();
        map <double, vector<string> >  getLog();
        void deliveryManager();
    private:
        int myid;
        int data;
        vector<int> vclock;
        time_t t_start;
        map <double, vector<string> > psuedoreceived;
        map <double, vector<string> > my_log;
        map <double, vector<string>, greater<double> > notdelivered;
        vector <int> D;
        vector< vector<int> > delay;
};

void receive (Process *, int);

int Process::getPortOf(int id){
    return ports[myid][id];
}

Process::Process(int id)
{
    delay = loadDelay();
    t_start = time(0);
    myid = id;
    data = 0;
    for (int i=0;i<ports.size();i++)
        {
            vclock.push_back(0);
            D.push_back(0);
        }

    return;
}
map <double, vector<string> > * Process::getPsuedoreceived()
{
    return &psuedoreceived;
}
map <double, vector<string> > Process::getLog()
{
    return my_log;
}
void Process::deliveryManager()
{
    for (const auto& p : notdelivered)
    {
        for(std::vector<string>::iterator it = p.second.begin(); it != p.second.end(); ++it) 
        {
            string message = (*it);
            int sender = getSender(message);
            int messagenum = getMessageNum(message);
            vector<int> ts = getVectorClock_vec(getMessageVC_str(message));

            if((D[sender]==ts[sender]-1))
            {
                bool deliver = true;
                for(int k=0; k<ports.size(); k++)
                {
                    if (k==j)
                        continue;
                    if(!(D[k]>=ts[k]))
                    {
                        deliver = false;
                        break;
                    }
                }
                if (deliver)
                {
                    //deliver. update log
                    D[sender] = ts[sender];
                    updateVectorClock_receive(getMessageVC_str(message));
                    
                vector<string> tokens = split(message, ',');    
                string toLog();
                toLog += "P";
                toLog += iToString(getId());
                toLog += " DLR p_";
                toLog += tokens[0];
                toLog += ":";
                toLog += tokens[1];
                double cutimeoff = difftime(time(0),parent->getStartTime());
                parent->addToLog(curtimeoff, toLog);


                    p.second.erase(it);


                    //add to log that delivered
                }
                // else
                // {
                //     No point to go down. can optimize.
                // }
            }
        }
    }
    return;
}

void Process::addToPsuedoreceived(double timeoffset, string t)
{
    psuedoreceived[timeoffset].push_back(t);
    return;
}

void Process::addToNotdelivered(double timeoffset, string t)
{
    notdelivered[timeoffset].push_back(t);

    return;
}

string Process::getVectorClock_str()
{
    string s = string();
    cout<<endl;
    for(int i=0;i<ports[0].size();i++)
    {
        if (i)
            {s += ".";}
        string vec_time_str = iToString(vclock[i]);
        s += vec_time_str;  
    }
    return s;
    
}

void Process::updateVectorClock_send()
{
    vclock[myid]++;
    string rv = "New vector clock is: ";
    rv+= getVectorClock_str();
    addToLog(rv);
    return;
}

void Process::updateVectorClock_receive(string str_ts_recmsg)
{
    vector<int> vc_ts_recmsg = getVectorClock_vec(str_ts_recmsg);
    for(int i=0;i<vc_ts_recmsg.size();i++)
    {
        if (vc_ts_recmsg[i]>vclock[i])
            vclock[i] = vc_ts_recmsg[i];
    }
    vclock[myid]++;
    string rv = "New vector clock is: ";
    rv+=getVectorClock_str();
    addToLog(rv);
    return;
}

time_t Process::getStartTime()
{
    return t_start;
}

void Process::addToLog(double toff, string s)
{
    log[toff].push_back(s);
    return;
}

int Process::getId()
{
    return myid;
}

int Process::getData()
{
    return data;
}
void Process::setData(int x )
{
    data = x;
}

void Process::sendBroadcast(int msg)
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    string tmp = iToString(getId());
    tmp += ",";
    tmp += iToString(msg);

    cout<<getId()<<" tmp "<<tmp<<endl;
    for(std::vector<int>::iterator it = ports[getId()].begin(); it != ports[getId()].end(); ++it) 
    {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) 
            error("ERROR opening socket");
        string host = "localhost";

        server = gethostbyname(host.c_str());
        // if (server == NULL) {
        //     fprintf(stderr,"ERROR, no such host\n");
        //     exit(0);
        // }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, 
             (char *)&serv_addr.sin_addr.s_addr,
             server->h_length);

        cout<<"sending to "<<*it<<endl;
        serv_addr.sin_port = htons(*it);
        if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
            error("ERROR connecting");
        
        bzero(buffer,256);
        
        string toSend= tmp;
        toSend += ",";

        //lock mutex?
        updateVectorClock_send();

        toSend += getVectorClock_str();
        toSend += ",";
        double timedifference = timediff(time(0) ,getStartTime());
        toSend += doubleToString(timedifference);
        cout<<"sending side"<<toSend<<endl;

        
        string logstring();
        logstring += "p";
        logstring += iToString(getId());
        logstring += " BRC p_";
        logstring+= iToString(getId());
        logstring += ":";
        logstring += iToString(msg);
        addToLog(timedifference, logstring);


        strcpy(buffer, toSend.c_str());
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
             error("ERROR writing to socket");
        
     }
    return;
}

void receive(Process* parent, int portno)
{
     int newsockfd, sockfd;
     
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     
     listen(sockfd,5); //5 max queue
     clilen = sizeof(cli_addr);

     string tolog = "Receiving starts on ";
     tolog+=iToString(portno);
     // logger<<tolog<<endl;
     parent->addToLog(tolog);
     while(1)
        {
             newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

             if (newsockfd < 0) 
                  error("ERROR on accept");
             bzero(buffer,256);
             n = read(newsockfd,buffer,255);
             if (n < 0) error("ERROR reading from socket");
             
             string buffer_data(buffer);
             vector<string> tokens = split(buffer_data, ',');
             tolog = "received from p"; 
             tolog += tokens[0];
             tolog += ": message ";
             tolog +=tokens[1];
             tolog += " with vector clock : ";
             tolog += tokens[2];
             tolog += " timeoffset :";
             tolog += tokens[3];


             //manage received messages
             int senderid = atoi(tokens[0].c_str());
             int receiverid = parent->getId();
             double send_timestamp = stod(tokens[3]);
             delay_m = parent->delay[senderid][receiverid];
             if(delay_m)
             {
                //add this message to psuedoreceived with key as the time it should come out. so delay + ts. 
                //todo: receiving manager thread
                parent->addToPsuedoreceived( (send_timestamp+stod(delay_m)) , buffer_data);
             }
             else
             {
                //give to delivery manager
                //add to log that received
                
                parent->addToNotdelivered( send_timestamp , buffer_data);  
                
                string toLog();
                toLog += "P";
                toLog += iToString(receiverid);
                toLog += " REC p_";
                toLog += tokens[0];
                toLog += ":";
                toLog += tokens[1];
                double cutimeoff = difftime(time(0),parent->getStartTime());
                parent->addToLog(curtimeoff, toLog);

                parent->deliveryManager();
             }

             

             parent->addToLog(tolog);
             cout<<tolog<<endl;
             
             //update clock



             // parent->setData(1);
             // n = write(newsockfd,"I got your message",18);
             // if (n < 0) error("ERROR writing to socket");
        }
    return;
}


// void Process::sendMsg(int dest_id, int msg)
// {
//     int sockfd, portno, n;
//     portno = getPortOf(dest_id);

//     struct sockaddr_in serv_addr;
//     struct hostent *server;

//     char buffer[256];

//     string tmp = iToString(getId());
//     tmp += ",";
//     tmp += iToString(msg);

//     bzero(buffer,256);
//     strcpy(buffer, tmp.c_str());
    
//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sockfd < 0) 
//         error("ERROR opening socket");
//     string host = "localhost";

//     server = gethostbyname(host.c_str());
//     // if (server == NULL) {
//     //     fprintf(stderr,"ERROR, no such host\n");
//     //     exit(0);
//     // }
//     bzero((char *) &serv_addr, sizeof(serv_addr));
//     serv_addr.sin_family = AF_INET;
//     bcopy((char *)server->h_addr, 
//          (char *)&serv_addr.sin_addr.s_addr,
//          server->h_length);

//     cout<<"About to connect for sending"<<endl;
//          serv_addr.sin_port = htons(portno);
//         if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
//             error("ERROR connecting");
        
//         n = write(sockfd,buffer,strlen(buffer));
//         if (n < 0) 
//              error("ERROR writing to socket");

//     //removing read after sending
//     // bzero(buffer,256);
//     // n = read(sockfd,buffer,255);
//     // if (n < 0) 
//     //      error("ERROR reading from socket");
//     // printf("%s\n",buffer);
//     return;
// }
