#include "../linkedlist/linkedList.c"
#include "../hashmap/hashmap.c"

struct posh {
   FILE * poshrc;
   FILE * poshrchistory;
   LinkedList * history;
   hashmap * globals;
   void * mainloop;
} typedef posh;

