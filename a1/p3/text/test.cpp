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
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

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


int main() {
	//map time to receiver
	std::map<int, vector<int> > uc_one;
	int senderid = 0;
	
	string line = "0 2 1,2";
	
	vector<string> tokens = split(line, ' ');
	int key1 = atoi(tokens[0].c_str());
	if (key1==senderid)
	{
		int key2 = atoi(tokens[1].c_str());
    	string timelist = tokens[2];
		vector<string> times = split(tokens[2], ',');
		// vector<int> times_int;
		for(vector<string>::iterator it = times.begin(); it != times.end(); ++it) 
    		{
        		
        		uc_one[atoi((*it).c_str())].push_back(key2);
    		}
	}
	
	map<int, vector<int> >::iterator iter;
	for(iter = uc_one.begin(); iter != uc_one.end(); iter++) 
    {
    	cout<<iter->first<<": ";
    	for(vector<int>::iterator it = iter->second.begin(); it != iter->second.end(); ++it) 
    		{
        		cout<<(*it)<<",";
    		}
    	cout<<endl;
    }



    line = "0 1 2";
    tokens = split(line, ' ');
    key1 = atoi(tokens[0].c_str());
    if (key1==senderid)
    {
        int key2 = atoi(tokens[1].c_str());
        string timelist = tokens[2];
        vector<string> times = split(tokens[2], ',');
        // vector<int> times_int;
        for(vector<string>::iterator it = times.begin(); it != times.end(); ++it) 
            {
                
                uc_one[atoi((*it).c_str())].push_back(key2);
            }
    }
    
    for(iter = uc_one.begin(); iter != uc_one.end(); iter++) 
    {
        cout<<iter->first<<": ";
        for(vector<int>::iterator it = iter->second.begin(); it != iter->second.end(); ++it) 
            {
                cout<<(*it)<<",";
            }
        cout<<endl;
    }





	return 0;
}