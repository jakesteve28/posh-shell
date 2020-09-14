#ifndef HASHMAP_H
#define HASHMAP_H
    typedef void * insertable;
    typedef int (*pairfunc)(insertable, insertable);
    typedef insertable hashmap; 
    extern hashmap newhashmap();
    extern int iteratehashmap(hashmap map, pairfunc callforeachpair, insertable item);
    extern int puthashmap(hashmap map, char * key, insertable value);
    extern insertable gethashmap(hashmap map, char * key);
    extern int removehashmap(hashmap map, char * key);
    extern void clearhashmap(hashmap map);
    extern int lengthhashmap(hashmap map);
#endif