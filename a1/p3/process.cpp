#include "include/process.h"

vector< vector<int> > ports;

Process::Process(int id, string type)
{   
    delay = loadDelay(type);
    t_start = time(0);
    myid = id;
    commtype = type;
    if (commtype=="Broadcast")
    {
        for (int i=0;i<ports.size();i++)
        {
            vclock.push_back(0);
            D.push_back(0);
        }
    }
    if(commtype=="Unicast")
    {
        for(int i=0;i<ports.size();i++)
        {
            vector<int> inside;
            for(int j=0;j<ports.size(); j++)
            {
                inside.push_back(0);
            }
            sent.push_back(inside);
            rec.push_back(0);
        }

    }

    return;
}

time_t Process::getStartTime()
{
    return t_start;
}

int Process::getPortOf(int id){
    return ports[myid][id];
}

void Process::addToLog(double toff, string s)
{
    my_log[toff].push_back(s);
    // cout<<"logged: "<<s<<endl;
    return;
}

int Process::getId()
{
    return myid;
}
string Process::getCommtype()
{
    return commtype;
}
vector< vector<int> > Process::getDelay()
{
    return delay;
}

map <double, vector<string> > * Process::getPsuedoreceived()
{
    return &psuedoreceived;
}

map <double, vector<string> > Process::getLog()
{
    return my_log;
}

void Process::addToPsuedoreceived(double timeoffset, string t)
{
    psuedoreceived[timeoffset].push_back(t);
    return;
}

void Process::addToNotdelivered(double timeoffset, string t)
{
    // cout<<"adding to notdelivered "<<timeoffset<<" "<<t<<endl;
    notdelivered[timeoffset].push_back(t);
    // cout<<((notdelivered.find(timeoffset))->second)[0]<<endl;
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

     // string tolog = ;
     // tolog+=iToString(portno);
     //cout<<"started listening "<<endl;
     while(1)
        {
             // cout<<"Receiving starts on "<<portno<<endl;
             newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

             if (newsockfd < 0) 
                  error("ERROR on accept");

             bzero(buffer,256);
             n = read(newsockfd,buffer,255);
             if (n < 0) error("ERROR reading from socket");
             
             string buffer_data(buffer);
             vector<string> tokens = split(buffer_data, ',');
             
             // cout<<"data : "<<buffer_data<<endl;
             
             int senderid = atoi(tokens[0].c_str());
             int receiverid = parent->getId();
             double send_timestamp = atof(tokens[3].c_str());
             double delay_m = parent->getDelay()[senderid][receiverid];
             if(delay_m)
             {
                parent->addToPsuedoreceived( (send_timestamp+(double)delay_m) , buffer_data);
             }
            else
             {
                parent->addToNotdelivered( send_timestamp , buffer_data);  
                
                string toLog = "P";
                toLog += iToString(receiverid);
                toLog += " REC p_";
                toLog += tokens[0];
                toLog += ":";
                toLog += tokens[1];
                // toLog += " with VC ";
                // toLog += tokens[2];
                double curtimeoff = difftime(time(0),parent->getStartTime());
                parent->addToLog(curtimeoff, toLog);
                parent->deliveryManager();
             }
        }
    return;
}

string Process::getRec_str()
{
    string s = string();
    for(int i=0;i<ports[0].size();i++)
    {
        if (i)
             {s += ".";}
        string vec_time_str = iToString(rec[i]);
        s += vec_time_str;  
    }
    return s;
}

string Process::getVectorClock_str()
{   
    string s = string();
    if (commtype=="Broadcast")
    {
        for(int i=0;i<ports[0].size();i++)
        {
            if (i)
                {s += ".";}
            string vec_time_str = iToString(vclock[i]);
            s += vec_time_str;  
        }
    
    }
    //case
    else
    {
        for(int i=0;i<ports.size();i++)
        {
            for(int j=0;j<ports.size(); j++)
            {
                if(i || j)
                {
                    s+=".";
                }       
                string vec_time_str = iToString(sent[i][j]);
                s += vec_time_str;  
            }
        }
    }

    return s;
    
}

void Process::updateVectorClock_sendBc()
{
    vclock[myid]++;
    return;
}

void Process::updateVectorClock_sendUc(int j)
{
    sent[myid][j] += 1;
    return;
}


void Process::deliveryManager()
{
    bool delivered = false;
    for(it_map iter = notdelivered.begin(); iter != notdelivered.end(); iter++) 
    {
        // cout<<iter->second[0]<<endl;

        for(int x = 0; x<iter->second.size();x++)
        {
        // for(std::vector<string>::iterator it = iter->second.begin(); it != iter->second.end(); ++it) 
        // {
            string message = iter->second[x];
            int sender = getSender(message);
            int messagenum = getMessageNum(message);

            if(commtype == "Broadcast")
            {
                vector<int> ts = getVectorClock_vec(getMessageVC_str(message));

                if((D[sender]==ts[sender]-1))
                {
                    bool deliver = true;
                    for(int k=0; k<ports.size(); k++)
                    {
                        if (k==sender)
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
                    string toLog = "P";
                    string temporary = iToString(getId());
                    toLog += temporary;
                    toLog += " DLR p_";
                    toLog += tokens[0];
                    toLog += ":";
                    toLog += tokens[1];
                    // toLog += " new VC ";
                    // toLog += getVectorClock_str();
                    double curtimeoff = difftime(time(0),getStartTime());
                    addToLog(curtimeoff, toLog);
                    // cout<<"delivered"<<endl;

                    iter->second.erase(iter->second.begin()+x);
                    delivered = true;
                    break;
                    }
                }
            }

            if(commtype == "Unicast")
            {
                vector< vector<int> > ts = getVectorClock_matrix(getMessageVC_str(message));
                // cout<<"in delivery manager"<<endl;
                if((rec[sender]==ts[sender][myid]-1))
                {
                    bool deliver = true;
                    // cout<<"in first condition"<<endl;
                    for(int k=0; k<ports.size(); k++)
                    {
                        if (k==sender)
                            continue;
                        if(!(rec[k]>=ts[k][myid]))
                        {
                            deliver = false;
                            break;
                        }
                    }
                    if (deliver)
                    {
                        // cout<<"delivering"<<endl;
                        //deliver. update log
                    rec[sender] = ts[sender][myid];
                    updateVectorClock_receive(getMessageVC_str(message));
                        
                    vector<string> tokens = split(message, ',');    
                    string toLog = "P";
                    string temporary = iToString(getId());
                    toLog += temporary;
                    toLog += " DLR p_";
                    toLog += tokens[0];
                    toLog += ":";
                    toLog += tokens[1];
                    // toLog += "; recvec: ";
                    // toLog += getRec_str();
                    // toLog += " new VC ";
                    // toLog += getVectorClock_str();
                    double curtimeoff = difftime(time(0),getStartTime());
                    addToLog(curtimeoff, toLog);
                    // cout<<"delivered"<<endl;

                    iter->second.erase(iter->second.begin()+x);
                    delivered = true;
                    break;
                    }
                }
            }

        }
    
    }
    if (delivered)
        deliveryManager();
    return;
}

void Process::updateVectorClock_receive(string str_ts_recmsg)
{
    if (commtype == "Broadcast")
    {
        vector<int> vc_ts_recmsg = getVectorClock_vec(str_ts_recmsg);
        for(int i=0;i<vc_ts_recmsg.size();i++)
        {
            if (vc_ts_recmsg[i]>vclock[i])
                vclock[i] = vc_ts_recmsg[i];
        }
    }
    else
    {   
        vector< vector<int> > msg_sent = getVectorClock_matrix(str_ts_recmsg);

        for(int i=0;i<ports.size();i++)
        {
            for(int j=0;j<ports.size(); j++)
            {
                if(msg_sent[i][j]>sent[i][j])
                {
                    sent[i][j] = msg_sent[i][j];
                }
            }
        }

    }
    return;    
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

    string logstring = "P";
    logstring += iToString(getId());
    logstring += " BRC p_";
    logstring += iToString(getId());
    logstring += ":";
    logstring += iToString(msg);
    // logstring += " with VC ";

    updateVectorClock_sendBc();
    // logstring += getVectorClock_str();
    double timedifference = difftime(time(0) ,getStartTime());
    addToLog(timedifference, logstring);
    
    
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

        
        serv_addr.sin_port = htons(*it);
        if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
            {
                perror("ERROR connecting");
                continue;
            }
        
        bzero(buffer,256);
        
        string toSend= tmp;
        toSend += ",";

        //lock mutex?
        

        toSend += getVectorClock_str();
        toSend += ",";
        
        toSend += doubleToString(timedifference);
        // cout<<"sending "<<toSend<<"to "<<*it<<endl;

        strcpy(buffer, toSend.c_str());
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
             error("ERROR writing to socket");
        
     }
    return;
}


//for unicast
void Process::sendUnicast(int dest, int msg)
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
 
    string tmp = iToString(getId());
    tmp += ",";
    tmp += iToString(msg);

    string logstring = "P";
    logstring += iToString(getId());
    logstring += "->P";
    logstring += iToString(dest);
    logstring += " p_";
    logstring += iToString(getId());
    logstring += ":";
    logstring += iToString(msg);

    updateVectorClock_sendUc(dest);
    // logstring += getVectorClock_str();
    double timedifference = difftime(time(0) ,getStartTime());
    addToLog(timedifference, logstring);
    
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

    
    serv_addr.sin_port = htons(getPortOf(dest));
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        {
            perror("ERROR connecting");
            return;
        }
    
    bzero(buffer,256);
    
    string toSend= tmp;
    toSend += ",";

    //lock mutex?

    toSend += getVectorClock_str();
    toSend += ",";
    
    toSend += doubleToString(timedifference);
    // cout<<"sending "<<toSend<<"to "<<dest<<endl;

    strcpy(buffer, toSend.c_str());
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    
    // cout<<"sent successfully"<<endl;
    return;
}


