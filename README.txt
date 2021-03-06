2013
James Ingalls
Ryan Leonard

Submitted Source Files:
phunt.c
action.h
action.c
actionlist.h
actionlist.c
log.h
log.c
util.h
util.c

Type "make" to compile the code. Type "make clean" to remove the compiled files. 


Work accomplished:
Created the phunt program as described. It is capable of parsing a config file, and performing all specified actions. It can also be run as a daemon by using the "-d" flag.


Descrption of Code and Layout:

The code is broken up into a few files for simplicity. phunt is the main program which parses the input and config file, and creates the actionlist as described in actionlist.c to be sent to action.c to be acted on. util.c contains a method to read in a line of a file, used in parsing the config (in actionlist.c) as well as the /proc filesystem in (action.c), and a method for handling our descriptive errors for debugging purposes used throughout.

There are two important functions in action.c - initAction(), and takeAction(). initAction() simply initialized the action structure for takeAction() depending on what type of action it is. phunt will call takeAction() separately, and repeatedly as long as it is running, with each action it has according to the config file. takeAction() iterates over all possible pids in proc, the max number defined as PID_MAX in action.h. This number may change depending on the system, but the number should be correct at least for ubuntu and ubuntu derived systems. If it is able to open a directory at the location of a given number, then we know there is a process with that pid. (We start at two so we can not kill init.) Once we have the directory open we check it for criteria based on the current action by reading the status file or the cwd path, and either setpriority() if we want to nice it, or send the process the appropriate signal in the case of a kill or suspend.


Comments:
We are assuming for the case of memory ceiling actions, that we will not kill or suspend unless the process EXCEEDS the ceiling. This also means that if it is actually equal to the ceiling we will nice for memory nice rules.

Number of hours: 
Approximately 36 man-hours.

Effort:
7.8/10

We would both like to use one grace day each for this assignment.