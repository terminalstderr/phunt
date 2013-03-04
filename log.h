/* 
Log.c provides an implementation of a logging method to files. Typical
usage for this program would be to log behaviour of a process. This supports
logging messages of the following form.
YYYY/MM/DD HH:MM:SS <prefix>: <message>
*/
#include <stdio.h>
#include <time.h>

#define MAX_STRLEN_TIME 32
#define MAX_STRLEN_LOGMESSAGE 256

struct Log{
   char *prefix;    // Information about this log which will print in every
                    // message logged.
   char *fileName;  // Name of the file that the log is writing to.
   int log_fd;      // File descriptor that we are writing to.
};



/*
Description: This will fill the fields of the log structure provided, and will
open the file indicated by the fileName parameter.

fileName: The name of the file which will be opened for logging.
prefixMessage: A message that will be printed with every line of the log.
log: The struct Log which will be initialized
Return: 0 if success, -2 if any parameter passed is incorrectly NULL, -3 if
unable to malloc sufficiant memory, -4 if ioError (could not open fileName).
*/
int initLog(char *fileName, char *prefixMessage, struct Log *log);



/*
Description: Write the provided message to the log using the format indicated at
the top of this file, and restated here:
YYYY/MM/DD HH:MM:SS <prefix>: <message>

message: String to be printed to the log.
log: A struct Log which has already been initialized.
Return: 0 if success, -1 if general memory Error, -2 if any parameter passed is
incorrectly NULL, -4 if ioError (could not write to log_fd).
*/
int writeMessage(char *message, struct Log *log);
