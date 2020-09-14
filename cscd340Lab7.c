#include "./pipes/pipes.c"
#include "./process/process.c"
#include "./tokenize/makeArgs.c"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "./poshrc/poshrc.c"

static void strip(char *array)
{
	if(array == NULL)
	{
		perror("array is null");
		exit(-99);
	}// end if

	int len = strlen(array), x = 0;
   
	while(array[x] != '\0' && x < len) {
	  if(array[x] == '\r')
		 array[x] = '\0';

	  else if(array[x] == '\n')
		 array[x] = '\0';

	  x++;
    }
}

static const char * DEFAULT_MESSAGE = "posh$: ";

FILE * poshrc;
FILE * poshrchistory;
const hashmap * globals;
const LinkedList * history;

static int argc, 
pipecount, ampersandcount, redirecttocount, execbg, 
redirectfromcount, pre = 0, post = 0, MAX_COMMAND = 256;

static void getposhrc(){
    poshrc = openrcfile(POSHRC); 
    poshrchistory = openrcfile(POSHRC_HISTORY_NAME);
    globals = getglobs(poshrc);
    history = gethistory(poshrchistory);
}

static void twopipe(){
//       int argc = makeargs(s, &argv);
//             char * check = "|";
//             char *** args = (char***)calloc(argc - 2, sizeof(char**));
            
            
//             for(int i = 0, m = 0; i < argc - 2; i++)
//             {
//                 int numcommandsatindex = sizeof(argv) / sizeof(argv[i]);
//                 args[i] = (char**)calloc(numcommandsatindex + 1, sizeof(char*));
//                 for(int j = 0; j < numcommandsatindex; j++)
//                 {
//                     if(strcmp(&argv[m][j], check) != 0)
//                     {
//                         args[i][j] = calloc(strlen(&argv[m][j]) + 1, sizeof(char));
//                         strcpy(args[i][j], &argv[m][j]);
//                     }
//                     else m++;
//                     continue;
//                     args[i][j + 1] = NULL;
//                 }
//             }
//             execPipe(args, 0, STDIN_FILENO);
//            // continue;
//         }
// }
}

static void onepipe(){
//  prePipe = parsePrePipe(s, &preCount);
//         postPipe = parsePostPipe(s, &postCount);
//         pipeIt(prePipe, postPipe);
//         clean(preCount, prePipe);
//         clean(postCount, postPipe);
}

static void execpipes(char * cmd){

}

static void runcmd(char * cmd){
    char ** argv = NULL;
    argc = makeargs(cmd, &argv);
    if(argc != -1)
        forkIt(argv, execbg);
    clean(argc, argv);

}

int main()
{ 
    getposhrc();	
    printf("%s", DEFAULT_MESSAGE);
    char * command_string = (char*)calloc(256, sizeof(char));
    fgets(command_string, MAX_COMMAND, stdin);
    strip(command_string);
    while(strcmp(command_string, "exit") != 0){
        pipecount = countchar(command_string, '|');    
        if(pipecount < 1) runcmd(command_string);
        else execpipes(command_string);
        printf("\r\n%s", DEFAULT_MESSAGE);
        char * command_res = fgets(command_string, MAX_COMMAND, stdin);
        strip(command_string);
    }
    return 0;
}// end main

