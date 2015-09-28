#include "include/thread_helper.h"

void write_log(Process* parent)
{
    string filepath = "logs/log_";
    filepath += iToString(parent->getId());
    
    while(1)
    {
        ofstream outfile(filepath.c_str());
        if (outfile.is_open())
        {
            // cout<<"fileopened"<<endl;
            map <double, vector<string> > p_log = parent->getLog();
            for(it_map iter = p_log.begin(); iter != p_log.end(); iter++) 
            {
                outfile<<"Time: "<<iter->first<<endl;
                for(std::vector<string>::iterator it = iter->second.begin(); it != iter->second.end(); ++it) 
                {
                    
                    outfile<<(*it)<<endl;
                }
                outfile<<endl;
            }
        }
        else
        {
            cout<<"couldn;t open file"<<endl;
        }
        outfile.close();
        sleep(1);
     }
    //for every 0.5s, write to file
    return;
}

void psuedoreceive_manager(Process* parent)
{
    while(1)
    {
        map <double, vector<string> > * psr = parent->getPsuedoreceived();
        for(it_map iter = psr->begin(); iter != psr->end(); iter++) 
        {
            double curtimeoff = difftime(time(0),parent->getStartTime());
            // cout<<curtimeoff<<endl;
            // cout<<iter->first<<endl;    
            if (iter->first<=curtimeoff)
            {
                for(std::vector<string>::iterator it = iter->second.begin(); it != iter->second.end(); ++it) 
                {
                    parent->addToNotdelivered(iter->first,(*it));

                vector<string> tokens = split((*it), ',');    
                string toLog = string();
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
                psr->erase(curtimeoff);
            }
        }
        sleep(1);
    }
    return;
}
