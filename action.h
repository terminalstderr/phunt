/*  2013 - James Ingalls <james.ingalls@gmail.com>
    2013 - Ryan Leonard <ryan.leonard71@gmail.com>
action.c provides an implmentation of a process manager based on the /proc/ 
filesystem. This is also based on the idea of taking an "action" on a subset of
processes that have a certain "attribute." The attributes are currently as
follows:
-Path: processes using ths provided path as CWD
-User: processes with the provided user
-Memory Usage: processes with the memory ceiling (in MB)

The actions that can be taken on the above stated items are:
-nice
-suspend
-kill
*/
#ifndef __ACTION_H__
#define __ACTION_H__

#define MAX_STRLEN_FILENAME 256
#define MAX_STRLEN_LINE     1024
#define PID_MAX             32768

extern struct Log *log;


// Defines the types of actions that can be taken
typedef short  action_t;
#define at_kill 0
#define at_susp 1
#define at_nice 2
extern const char *actionStrings[3];
// = { "kill", "suspend", "nice" };


// Defines the types of parameters that may be passed
typedef short param_t;
#define pt_user 0
#define pt_path 1
#define pt_mem  2
extern const char *paramStrings[3];
// = { "user", "path", "memory" };

// Defines the parameter itself
typedef union{
    char *pathName;
    int uid;
    int memoryCap;  // in kB
} Parameter;

struct Action{
    action_t actionType;
    param_t paramType;
    Parameter param;
};

/*
This will simply initialize all of the fields in the action struct.

type: The type of action which will be taken.
param: Depending on the type, this will be parsed accordingly.
action: The action to be initialized

Return: 0 upon success, -6 upon invalid parameter passed
*/
int initAction(action_t at, param_t pt, char *param, struct Action *action);



/*
Takes the File of the first processas input, will iterate through pids in /proc until 
it has checked each one whether it should take action on it. Criteria to 
check will depend on current action.
*/

int actionLoop(int fd);



/*
Description: Given an action, enforce that action.

This function will have to look at the /proc
filesystem every time it runs.


Return: -6 upon invalid parameter passed
*/
int takeAction(struct Action *action);
    
/*
Description: A helper function for people who are building actions. This will take
a character array and let the user know via the return value what type of parameter
the passed string matches.

Return: the parameter type on success, -2 if incorrect parameter passed
*/
param_t getParamType(char *paramTypeStr);

/*
Description: A helper function for people who are building actions. This will take 
a character array and let know which of our action types it matches via the return
value.

Return: the action type on success, -2 if incorrect parameter passed
*/
action_t getActionType(char *actionTypeStr);



/*
Description: Based on the action prints the intended behavior to the log.

*/
int actionToLog(char *out, struct Action *action);



/*
*/
int getUidFromUser(char *uname);



/*
*/
int getUIDFromProcess(char *uname);



/*
Description: Called when action is taken 

*/
int actionTakenToLog(char *out, struct Action *action);


/*
Description: Frees the allocated fields of an action, as well as 
*/
int freeAction(struct Action *action);

#endif//__ACTION_H__
