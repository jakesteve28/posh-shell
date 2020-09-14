#ifndef POSHRC_CONFIG_H
#define POSHRC_CONFIG_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <errno.h>
    #include <assert.h>
    #include <regex.h>
	#include "../hashmap/hashmap.c"
	#include "../linkedlist/linkedList.c"
    extern int POSHRC_HISTORY;
    extern int HISTORY_FILE_COUNT;
    FILE * openrcfile(char * filename);
    hashmap * getglobs(FILE * poshrcfile);
    LinkedList * gethistory(FILE * historyfile);
    char * POSHRC = ".poshrc";
    char * POSHRC_CREATE_NEW = "Creating new file...\r\n";
    char * POSHRC_FILE_OPEN_REGEX = "[[:alnum:]]{125}=[[:alnum:]]{125}";
    char * POSTRC_HISTORY_FILE_REGEX = "[[:alnum:]]{125}";
    char * POSHRC_FILE_LINE_ERROR = "Could not compile regex";
    char * POSHRC_HISTORY_NAME = ".posh_history";
    char * POSHRC_OPEN_ERROR = "WARNING: .poshrc/.posh_history file open error: "; 
#endif 