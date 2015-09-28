#include "include/main_header.h"
int main(int argc, char *argv[])
{
    loadPorts();

    int id = 4;
    string commtype = "Unicast";
    Process p(id, commtype);

    boost::thread_group workers;
    for (int i=0; i<ports.size();i++)
    {
        workers.add_thread(new boost::thread(receive, &p, ports[i][id]));
    }
    workers.add_thread(new boost::thread(write_log, &p));
    workers.add_thread(new boost::thread(psuedoreceive_manager, &p));
    sleep(2);
    int messagecounter = 1;

    //case
    if(commtype=="Broadcast")
    {
        map<int, vector<int> > bc_all = loadBc();
        vector <int> my_time = bc_all[id];

        while(!my_time.empty())
        {
            int nexttime = my_time.front();
            double curr = difftime(time(0),p.getStartTime());
            int cur = (int) curr;
            cur-=2;
            if (cur==nexttime)
            {
                p.sendBroadcast(messagecounter);
                messagecounter++;
                cout<<"Sending at "<<cur<<endl;
                my_time.erase(my_time.begin());
            }
            
            else
            {
                sleep((my_time.front()-cur));
            }
        }
    }

    
    if(commtype=="Unicast")
    {
        map<int, vector<int> > uc = loadUc(id);
        while(!uc.empty())
        {
            double curr = difftime(time(0),p.getStartTime());
            int cur = (int) curr;
            cur-=2;
            
            map<int, vector<int> >::iterator it; 
            it = uc.find(cur);
            if (it != uc.end())
            {
                for (vector<int>::iterator iter = it->second.begin();iter!=it->second.end();++iter)
                {

                    cout<<"to send to "<<*iter<<" at "<<cur<<endl;
                    p.sendUnicast(*iter, messagecounter);
                }
                uc.erase (it);
            }
            else
            {
                sleep(1);
            }
        }
    }
    workers.join_all();
    return 0; 
}
