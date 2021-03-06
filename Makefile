CC			= gcc
PROJECTDIR 	= .
CFLAGS		= -Wall
DEBUG		= -g -DDEBUG

default: phunt

debug: CFLAGS += $(DEBUG)
debug: default

test_util:
	$(CC) test_util.c util.c $(CFLAGS) -g -DDEBUG -o $(PROJECTDIR)/test_util

test_log:
	$(CC) test_log.c log.c $(CFLAGS) -g -DDEBUG -o $(PROJECTDIR)/test_log

test_action:
	$(CC) test_action.c action.c util.c log.c $(CFLAGS) -g -DDEBUG -o $(PROJECTDIR)/test_action

test_actionlist:
	$(CC) test_actionlist.c actionlist.c action.c util.c log.c $(CFLAGS) -g -DDEBUG -o $(PROJECTDIR)/test_actionlist

phunt: phunt.c
	$(CC) phunt.c actionlist.c action.c util.c log.c $(CFLAGS) -o $(PROJECTDIR)/phunt

clean:
	rm -f *.o
	@ rm $(PROJECTDIR)/phunt $(PROJECTDIR)/test_util $(PROJECTDIR)/test_log $(PROJECTDIR)/test_actionlist $(PROJECTDIR)/test_action 
