// 2013 - Ryan Leonard <ryan.leonard71@gmail.com>
// 2013 - James Ingalls <james.ingalls@gmail.com>
#include "util.h"
#include "log.h"
#include "actionlist.h"
#include "action.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_OPT_LEN 256
extern struct Log *log;


/*
Description: Macro for printing errors IN THE MAIN METHOD ONLY
*/
#define printError(errorCode) do{               \
    printf("Error: %s -- %s:%d\n    Message: %s\n", __FILE__, __func__, __LINE__, strError(errorCode));  \
}while(0)

/*
Description: Prints the usage to user.
*/
static void usage(char **argv);



/* 
Main should do all initialization and interaction with the user. It should
also in the case of the request of running as a daemon, be forked and moved 
to the appropriate environment. E.g. change current directory, changed
parent process to the init process, etc...
*/
int main(int argc, char **argv){

    int 
        ret,            // general-purpose/error-checking return value
        isDaemon = 0;   // flag to indicate that this program will execute as a daemon

    char
        logFileName[MAX_OPT_LEN] = "/var/log/phunt.log",    // Filename of log to append to while running this process
        configFileName[MAX_OPT_LEN] = "/etc/phunt.conf",    // Filename of configuration for running this process
        *logPrefix = "phunt",                               // The prefix which is printed with every log message, at most 32 bytes
        logMessage[MAX_STRLEN_LOGMSG];                      // A buffer for composing messages to write to log

    struct Action *action;  // An action pointer used when interacting with the actionList

    struct ActionList 
        *actionList,        // The main actionList
        *actionList_iter;   // Iterator for the main actionList

    struct Log phunt_log;   // The log structure for the phunt process
    log = &phunt_log;       // Set the global, externed log to point to the phunt process's log

#ifdef DEBUG
    printf("===\nRunning %s in Debug mode.\n===\n\n", argv[0]);
#endif



    ///// Usage & Parse parameters
    // Parse options provided at CL
    while ((ret = getopt(argc, argv, "l:c:d")) != -1)
    {
        switch (ret)
        {
            case 'l':
                strncpy(logFileName, optarg, MAX_OPT_LEN);
                logFileName[MAX_OPT_LEN-1] = '\0';
                break;
            case 'c':
                strncpy(configFileName, optarg, MAX_OPT_LEN);
                configFileName[MAX_OPT_LEN-1] = '\0';
                break;
            case 'd':
                isDaemon = 1;
                break;
            default:
                usage(argv);
                exit(0);
        }
    }



    ///// Daemonize if we are asked to
    writeMessage("Running phunt as a daemon, forking now.", log);
    if (isDaemon == 1){
        ret = fork();
        if (ret > 0){ // if we are the parent
            exit(0); 
        }
        if (ret < 0){ // if there was an error
            writeMessage("Error deamonizing phunt:", log);
            writeMessage(strError(ret), log);
            exit(0); 
        }
    }



    ///// Initialize log and actionList
    // initializt the log 
    ret = initLog(logFileName, logPrefix, log);
    if (ret < 0){
        printError(ret);
        // writeMessage("Error initializing log", log);
        // writeMessage(strError(ret), log);
        exit(0);
    }
    sprintf(logMessage, "Initialized log, logging to file: %s", logFileName);
    writeMessage(logMessage, log);

    // initialize the actionList
    sprintf(logMessage, "Initializing phunt via config file: %s", configFileName);
    writeMessage(logMessage, log);
    ret = initActionList(configFileName, &actionList);
    if (ret < 0){
        printError(ret);
        writeMessage("Error initializing phunt:", log);
        writeMessage(strError(ret), log);
        exit(0);
    }

    // Print startup message
    sprintf(logMessage, "Phunt initialized and executing (PID=%d)", getpid());
    writeMessage(logMessage, log);



    ///// Event Loop
    while(1){
        // Run through the list of actions and fulfill (takeAction) each
        actionList_iter = actionList;
        do{
            ///// Get the next action in the list
            ret = getAction(&action, &actionList_iter);
            if (ret < 0){
                printError(ret);
                writeMessage("Error getting next action from actionList:", log);
                writeMessage(strError(ret), log);
                exit(0);
            }
            // Incrament our list to the next action
            ret = nextAction(&actionList_iter);
            if (ret < 0){
                printError(ret);
                writeMessage("Error iterating through actionList:", log);
                writeMessage(strError(ret), log);
                exit(0);
            }

            ///// fulfil the next action
            if (action->paramType == pt_path)
                sprintf(logMessage, "Executing action = (%d, %d, %s)", action->actionType, action->paramType, action->param.pathName);
            else if (action->paramType == pt_user)
                sprintf(logMessage, "Executing action = (%d, %d, %d)", action->actionType, action->paramType, action->param.uid);
            else// if (action->paramType == pt_mem)
                sprintf(logMessage, "Executing action = (%d, %d, %d)", action->actionType, action->paramType, action->param.memoryCap);
            writeMessage(logMessage, log);
            ret = takeAction(action);
            if (ret < 0){
                printError(ret);
                writeMessage("Error executing action (takeAction):", log);
                writeMessage(strError(ret), log);
                exit(0);
            }
        } while ( actionList_iter != actionList );
    }



    ///// Clear up all resources used
    // clear memory from actionList
    ret = freeActionList(&actionList_iter);
    if (ret < 0){
        printError(ret);
        writeMessage("Error freeing actionList:", log);
        writeMessage(strError(ret), log);
    }
    // release open log resources
    ret = closeLog(log); 
    if (ret < 0){
        printError(ret);
        writeMessage("Error closing log:", log);
        writeMessage(strError(ret), log);
    }

    // DONE!!!
    sprintf(logMessage, "Phunt has completed execution and will now halt (PID=%d)", getpid());
    writeMessage(logMessage, log);
    exit(0);
}//END main



static void usage(char **argv)
{
    printf("Usage: ");
    if (argv != NULL && argv[0] != NULL)
        printf("%s [-l <log_file>] [-c <config_file>] [-d]\n", argv[0]);
    else
        printf("phunt [-l <log_file>] [-c <config_file>] [-d]\n");
}
