Assignment 1
Problem 3 and 4
Rahul R Huilgol, rrh2226, rahulhuilgol@gmail.com


Compiling instructions:
	Type 'make'
-----
Running instructions:
	Type './run.sh'
	When asked enter 'b' or 'u' and press enter for Broadcast and Unicast modes 		respectively.
-----
Output:
	Program generates logs per process and merges them at the end. Per process logs are in logs folder. When run.sh exits, log_merger.py merges them into merged_u.log or merged_b.log depending on mode run. The log structure is similar to the example given to us. If any intermediate row (particular time) has no event, it is not shown in log.
-----
Directory structure:
The presence of empty folders bin and logs is necessary.
	bin/ - executables
	config/ - config files
	include/ - headers
	logs/ - per process log files

-----	
Config files:

bc.txt: Stores at what time a process is supposed to broadcast messages. There should be no empty line at end of file
	Format: O bc at 1,2
	means process 0 broadcasts at t=1 and t=2
	
uc.txt: Stores when and to whom a process sends unicast message. There should be no empty line at end of file. 
	Format: 0 2 1,2
	means process 0 sends message to 2 process 2 at t=1 and t=2

ports.txt: This only needs to be changed when the number of processes interacting is changed. This file is used by program to determine how many processes are executing. To change number of processes, the run.sh file also needs to be changed to the new number of processes.
	Format: a,b,c,d
	If 4 processes are executing, then 4 comma separated port numbers.

delay_bc.txt: Sets delay. Structure similar to the example shared on piazza. It is upper triangular. Meaning if (i,j) is set in the upper traingle, then (j,i) uses the same value.

delay_uc.txt: Same as above. Just for unicast.

All configs are currently the same as in the examples shared on piazza.

delay_bc.txt: Sets delay. Structure similar to the example shared on piazza


-----
Note: Sometimes on running it may say, address already in use. This is because the previous run of the program hasn't freed the sockets yet. Exiting with Ctrl C kills all the processes that were running. It still takes few seconds for the ports to be free. Its best to wait half a minute before trying again.

If the exit was not with Ctrl-C there is a chance the processes may still be running. Find them using 'lsof -P -n | grep "bin/main"' and do 'kill -9 pid1 pid2...' for pids you see there.
