#include "include/helper.h"



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

void loadPorts()
{
    string filepath = "config/ports.txt";
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


vector<vector<int> > loadDelay(string commtype)
{
    vector<vector <int> > delay;
    string filepath;
    if (commtype == "Broadcast")
    {
        filepath = "config/delay_bc.txt";
    }
    else
    {
        filepath = "config/delay_uc.txt";  
    }
        
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
    delay_data.open(filepath.c_str());
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

        for(int i=0;i<numrows;i++)
        {
            for (int j=i;j<numcols;j++)
            {
                delay[j][i] = delay[i][j];
            }
        }


      }

    else
    {
        cout<<"failed to open file"<<endl;
    }
    return delay;
}



map<int, vector<int> > loadBc()
{
    map<int, vector<int> > bc;

    string filepath = "config/bc.txt";
    
    string line;
    int num;
    ifstream bc_data(filepath.c_str());
    if (bc_data.is_open())
      {
        while (getline(bc_data,line))
        {
            vector<string> tokens = split(line, ' ');
            int forpid = atoi(tokens[0].c_str());
            string timelist = tokens[3];

            vector<int> rval;
            vector<string> times = split(tokens[3], ',');
            for(std::vector<string>::iterator it = times.begin(); it != times.end(); ++it) 
            {
                // cout<<*it<<endl;
                rval.push_back(atoi((*it).c_str()));
            }
            bc[forpid] = rval;
        }
        bc_data.close();


        // map<int, vector<int> >::iterator itera;
        // for(itera = bc.begin(); itera != bc.end(); itera++) 
        // {
        //     cout<<itera->first<<": ";
        //     for(std::vector<int>::iterator it = itera->second.begin(); it != itera->second.end(); ++it) 
        //     {
        //         cout<<(*it)<<" ";
        //     }
        //     cout<<endl;
        // }



      }

    else
    {
        cout<<"failed to open file"<<endl;
    }
    return bc;
}


map<int, vector<int> > loadUc(int senderid)
{
    
    map<int, vector<int> > uc_one;

    string filepath = "config/uc.txt";
    
    string line;
    int num;
    ifstream uc_data(filepath.c_str());
    if (uc_data.is_open())
    {
        while (getline(uc_data,line))
        {
            vector<string> tokens = split(line, ' ');
            int key1 = atoi(tokens[0].c_str());
            if (key1==senderid)
            {
                int key2 = atoi(tokens[1].c_str());
                string timelist = tokens[2];
                vector<string> times = split(tokens[2], ',');
                for(vector<string>::iterator it = times.begin(); it != times.end(); ++it) 
                    {
                        uc_one[atoi((*it).c_str())].push_back(key2);
                    }
            }
            
        }
        uc_data.close();

        // map<int, vector<int> >::iterator iter;
        // for(iter = uc_one.begin(); iter != uc_one.end(); iter++) 
        // {
        //     cout<<iter->first<<": ";
        //     for(vector<int>::iterator it = iter->second.begin(); it != iter->second.end(); ++it) 
        //     {
        //         cout<<(*it)<<",";
        //     }
        //     cout<<endl;
        // }

      }

    else
    {
        cout<<"failed to open file"<<endl;
    }
    return uc_one;
}


vector< vector<int> > getVectorClock_matrix(string tclock)
{
    vector< vector<int> > msg_sent;
    vector<string> tokens = split( tclock, '.');
    for(int i=0;i<ports.size();i++)
    {
        vector<int> inside;
        for(int j=0;j<ports.size(); j++)
        {
            string val = tokens[(i*ports.size())+j];
            inside.push_back(atoi(val.c_str()));
        }
        msg_sent.push_back(inside);
    }

    return msg_sent;  

}