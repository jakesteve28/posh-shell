#include "process.h"

static const int maxdirsize = 1024;
static char * workingdir;

void forkIt(char **args, int background){	 
    workingdir = (char*) calloc(1024, sizeof(char));
    int err = -1;
    pid_t pid;
    int status;
    if(( pid = fork() ) == -1){
        printf("FATAL ERROR: Child process could not be created\n");
        return;
    }
    if(pid == 0){	
        signal(SIGINT, SIG_IGN);
        setenv("child", getcwd(workingdir, maxdirsize), 1);	
        if (execvp(args[0],args) == -1){
            printf("Command not found: Check globals or current directory");
            kill(getpid(),SIGTERM);
        }
    }
    if (background == 0){
        waitpid(pid, &status, 0);
    }
}

void runbg(char ** args, int background){
    
}

