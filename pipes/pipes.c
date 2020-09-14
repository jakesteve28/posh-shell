#include "pipes.h"
#include "../tokenize/makeArgs.h"
#include <string.h>

int countchar(char * s, char tok)
{
    int i = 0, count = 0, length = strlen(s);
    for( ; i < length; i++)
    {
        if(*(s + i) == tok)
        {
            count++;
        }
    }
    return count;
}// end containsPipe


char ** parsePrePipe(char *s, int * preCount)
{
    char * preString = (char*)calloc(100, sizeof(char));
    int i;
    for(i = 0; s[i] != '|'; i++)
    {
        preString[i] = s[i];
    }
    preString[i + 1] = '\0';
    char *** ret = (char***)calloc(1, sizeof(char**));
    int length = makeargs(preString, ret);
    printargs(length, *ret);
    free(preString);
    preString = NULL;
    return *ret;
}// end parsePrePipe


char ** parsePostPipe(char *s, int * postCount)
{
    char * postString = (char*)calloc(100, sizeof(char));
    int length = strlen(s);
    int i = 0;
    for( ; s[i] != '|'; i++);
    i++;
    int j;
    for(j = 0; i < length; i++, j++)
    {
        postString[j] = s[i];
    }
    postString[j+1] = '\0';
    char *** ret = (char***)calloc(1, sizeof(char**));
    int size = makeargs(postString, ret);
    printargs(size, *ret);
    free(postString);
    postString = NULL;
    return *ret;
}// end parsePostPipe


void pipeIt(char ** prePipe, char ** postPipe)
{  
    pid_t pid;
    int fd[2], in = 0, out = 1;
    pipe(fd);
    pid = fork();
    if(pid==0)
    {
        dup2(fd[out], 1);
        close(fd[in]);
        close(fd[out]);
        execvp(prePipe[0], prePipe);
        exit(1);
    }
else
{ 
    pid=fork();
    if(pid == 0)
    {
        dup2(fd[in], 0);
        close(fd[out]);
        close(fd[in]);
        execvp(postPipe[0], postPipe);
        exit(1);
    }
    else
    {
        int status;
        close(fd[out]);
        close(fd[in]);
        waitpid(pid, &status, 0);
    }
}// end pipeIt
}
void execPipe(char *** commands, size_t position, int in_fd)
  {
      if(commands[position + 1] == NULL) //Last command in pipe base case
      {
          dup2(in_fd, STDIN_FILENO); //piping from in_fd to stdout
          close(in_fd);
          execvp(commands[position][0], commands[position]);
          exit(-99); // if failed, bail
      }
      else
      {
          int fd[2];
          if(pipe(fd) == -1)
          {
              printf("Bad pipe");
              exit(-99);
          }
          pid_t pid;
          switch(pid = fork())
          {
              case -1: printf("Fork failure"); exit(-99);
              case 0: //Child execute current command
                  close(fd[0]);
                  dup2(in_fd, STDIN_FILENO);
                  dup2(fd[1], STDOUT_FILENO);
                  execvp(commands[position][0], commands[position]);
                  exit(-99); //bad child
              default:
                  close(fd[1]);
                  close(in_fd);
                  execPipe(commands, position + 1, fd[0]); //Pass fd[0] which bridges between each pipe
          }
      }
  }

