#include "./poshrc.h"

static regex_t regex;
static int reti; 
static char regexerrorbuffer[256];

static void failhard(){
    perror("FATAL ERROR: Terminating posh");
    exit(-1);
}

static int classifyregex(int reti){
    if(!reti){
        return 1;
    } else if (reti == REG_NOMATCH) {
        return 0;
    } else {
        regerror(reti, &regex, regexerrorbuffer, sizeof(regexerrorbuffer));
        return 0;
    }
}

static void printopenerror(){
    char * errnomessage = strerror(errno);
    char * errormessage = strcat(POSHRC_OPEN_ERROR, errnomessage);
    printf("%s\r\n", errormessage);
    printf("%s\r\n", POSHRC_CREATE_NEW);
}

FILE * openrcfile(char * filename){
    FILE * fp = fopen(POSHRC, "a+");
    if(fp == NULL){
        printopenerror();
        fp = fopen(POSHRC, "w");
    } 
    return fp;
}

hashmap * getglobs(FILE * poshrcfile){
    assert(poshrcfile != NULL);
    char buf[256];
    hashmap globals = newhashmap();
    while(fgets(buf, 256, poshrcfile) != NULL){
        int regex_res = 1;
        if(regex_res){
            char * split = "=";
            char * key = strtok(buf, split);
            char * val = strtok(NULL, split);
            int hashmapput = puthashmap(globals, key, val);
            if(hashmapput == -1) continue;
        } else {
            continue;
        }
    }
    return globals;
}

 char *safe_strcpy(char *dest, size_t size, char *src) {
      if (size > 0) {
          size_t i;
          for (i = 0; i < size - 1 && src[i]; i++) {
               dest[i] = src[i];
          }
          dest[i] = '\0';
      }
      return dest;
  }

LinkedList * gethistory(FILE * historyfile){
    assert(historyfile != NULL);
    char buf[256];
    LinkedList * history = linkedList();
    while(fgets(buf, 256, historyfile) != NULL){
        Node * historynode = (Node *)calloc(1, sizeof(Node));
        historynode->data = (char*)calloc(256, sizeof(char));
        strcpy(historynode->data, buf);
        //strcpy(historynode->data, buf);
        addFirst(history, historynode);
    }
    return history;
}

void addhistory(LinkedList * list, FILE * historyfile, char * cmd){
    Node * node = (Node*)malloc(sizeof(Node));
    node->data = (char*)calloc(512, sizeof(char));
    strcpy(node->data, cmd);
    addLast(list, node);
    fprintf(historyfile, "%s\n", cmd);
}
