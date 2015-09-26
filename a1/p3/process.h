/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
//g++ -w server.cpp -o server -lboost_thread-mt
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

using namespace std;

vector<int> ports;

string iToString(int s)
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
//end of fns to split string

void my_handler(int s){           
           //printf("Terminating on catching signal %d\n",s);
           exit(0); 
}

void error(char *msg)
{
    perror(msg);
    exit(1);
}

void loadPorts()
{
    string port;
    int portnum;
    ifstream portdata("ports.txt");
    if (portdata.is_open())
      {
        while ( getline (portdata,port) )
        {
          portnum = atoi(port.c_str());
          ports.push_back(portnum);
        }
        portdata.close();
      }
    return;
}


class Process{
    public:
        Process(int);
        // void setUpReceiveChannel();
        int getId();
        int getData();
        void setData(int);
        int getPortOf(int);
        vector<int> getPorts();
        void sendMsg(int, int);
        void sendBroadcast(int);
        // void deliveryManager();
        // int getSock();
        // void setSock();
    private:
        int myid;
        int data;
        // int sockfd;

};

void receive (Process *);

int Process::getPortOf(int id)
{
    return ports[id];
}

Process::Process(int id)
{
    myid = id;
    data = 0;
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
    // portno = getPortOf(dest_id);

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    string tmp = iToString(getId());
    tmp += ",";
    tmp += iToString(msg);
    
    bzero(buffer,256);
    strcpy(buffer, tmp.c_str());
    
    for(std::vector<int>::iterator it = ports.begin(); it != ports.end(); ++it) 
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

    //have to do below for every process in broadcast

  
        cout<<"sending to "<<*it<<endl;
         serv_addr.sin_port = htons(*it);
        if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
            error("ERROR connecting");
        
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
             error("ERROR writing to socket");

     }


    //removing read after sending
    // bzero(buffer,256);
    // n = read(sockfd,buffer,255);
    // if (n < 0) 
    //      error("ERROR reading from socket");
    // printf("%s\n",buffer);
    return;
}

void Process::sendMsg(int dest_id, int msg)
{
    int sockfd, portno, n;
    portno = getPortOf(dest_id);

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    string tmp = iToString(getId());
    tmp += ",";
    tmp += iToString(msg);

    bzero(buffer,256);
    strcpy(buffer, tmp.c_str());
    
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

    //have to do below for every process in broadcast

    // for(std::vector<int>::iterator it = ports.begin(); it != ports.end(); ++it) 
    // {

    cout<<"About to connect for sending"<<endl;
         serv_addr.sin_port = htons(portno);
        if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
            error("ERROR connecting");
        
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
             error("ERROR writing to socket");
    // }


    //removing read after sending
    // bzero(buffer,256);
    // n = read(sockfd,buffer,255);
    // if (n < 0) 
    //      error("ERROR reading from socket");
    // printf("%s\n",buffer);
    return;
}

void receive(Process* parent)
{
     string idpart = iToString(parent->getId());
     string filenam = "log_";
     filenam += idpart;
     ofstream logger(filenam.c_str());

     int portno = parent->getPortOf(parent->getId());
     int newsockfd, sockfd;
     
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

    // parent->setSock(sockfd);
     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     
     listen(sockfd,5); //5 max queue
     clilen = sizeof(cli_addr);

     logger<<"Receiving about to start on "<<portno<<endl;
     while(1)
        {
             newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
             //cout<<cli_addr.sin_port<<" "<<cli_addr.sin_addr.s_addr<<endl;

             if (newsockfd < 0) 
                  error("ERROR on accept");
             bzero(buffer,256);
             n = read(newsockfd,buffer,255);
             if (n < 0) error("ERROR reading from socket");
             

             //printf("P%d ",parent->getId());
             string buffer_data(buffer);
             vector<string> tokens = split(buffer_data, ',');
             logger<<"received from p"<<tokens[0]<<" message "<<tokens[1]<<endl;
             parent->setData(1);
             // n = write(newsockfd,"I got your message",18);
             // if (n < 0) error("ERROR writing to socket");
        }
    return;
}
